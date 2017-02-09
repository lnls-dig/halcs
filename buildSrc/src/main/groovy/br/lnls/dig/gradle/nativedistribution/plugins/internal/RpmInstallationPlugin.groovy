package br.lnls.dig.gradle.nativedistribution.plugins

import org.gradle.api.distribution.Distribution
import org.gradle.api.internal.project.ProjectIdentifier
import org.gradle.api.internal.resolve.ProjectModelResolver
import org.gradle.api.Project
import org.gradle.api.Task
import org.gradle.internal.service.ServiceRegistry
import org.gradle.model.Each
import org.gradle.model.internal.core.ModelNode.State
import org.gradle.model.internal.core.ModelPath
import org.gradle.model.internal.type.ModelType
import org.gradle.model.Model
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.model.internal.RpmDistribution
import br.lnls.dig.gradle.nativedistribution.tasks.RpmInstall
import br.lnls.dig.gradle.nativedistribution.tasks.RpmUninstall

class RpmInstallationPlugin extends RuleSource {
    @Mutate
    public void addInstallationTasks(ModelMap<Task> tasks,
            DistributionContainer distributions,
            @Path("buildDir") File buildDir) {
        distributions.all { distribution ->
            tasks.create(distribution.taskNameFor("installRpm"),
                    RpmInstall) { task ->
                task.distribution = distribution
                task.rpmDirectory = new File(buildDir,
                        "/distributions/$distribution.name")
            }
        }
    }

    @Mutate
    public void addUninstallationTasks(ModelMap<Task> tasks,
            DistributionContainer distributions,
            @Path("buildDir") File buildDir) {
        distributions.all { distribution ->
            tasks.create(distribution.taskNameFor("uninstallRpm"),
                    RpmUninstall) { task ->
                task.distribution = distribution
            }
        }
    }

    @Model
    public void distributionsGoingBackward(
            ModelMap<Distribution> distributionsGoingBackward,
            ProjectIdentifier project,
            @Path('distributions') DistributionContainer currentDistributions,
            @Path('distributionsFromProjectsBefore')
                ModelMap<Distribution> distributionsFromBefore) {
        distributionsFromBefore.keySet().each { key ->
            def distribution = distributionsFromBefore.get(key)

            distributionsGoingBackward.put(key, distribution)
        }

        currentDistributions.each { distribution ->
            def name = "$project.path:$distribution.name"

            distributionsGoingBackward.put(name, distribution)
        }
    }

    @Model
    public void distributionsGoingForward(
            ModelMap<Distribution> distributionsGoingForward,
            ProjectIdentifier project,
            @Path('distributions') DistributionContainer currentDistributions,
            @Path('distributionsFromProjectsAfter')
                ModelMap<Distribution> distributionsFromAfter) {
        distributionsFromAfter.keySet().each { key ->
            def distribution = distributionsFromAfter.get(key)

            distributionsGoingForward.put(key, distribution)
        }

        currentDistributions.each { distribution ->
            def name = "$project.path:$distribution.name"

            distributionsGoingForward.put(name, distribution)
        }
    }

    @Model
    public void distributionsFromProjectsBefore(
            ModelMap<Distribution> distributionsFromProjectsBefore,
            ProjectIdentifier project, ServiceRegistry serviceRegistry) {
        def resolver = serviceRegistry.get(ProjectModelResolver)
        def distributions = getDistributionsBefore(project, resolver)

        distributions?.keySet()?.each { key ->
            distributionsFromProjectsBefore.put(key, distributions.get(key))
        }
    }

    @Model
    public void distributionsFromProjectsAfter(
            ModelMap<Distribution> distributionsFromProjectsAfter,
            ProjectIdentifier project, ServiceRegistry serviceRegistry) {
        def resolver = serviceRegistry.get(ProjectModelResolver)
        def distributions = getDistributionsAfter(project, resolver)

        distributions?.keySet()?.each { key ->
            distributionsFromProjectsAfter.put(key, distributions.get(key))
        }
    }

    private ModelMap<Distribution> getDistributionsBefore(
            ProjectIdentifier project, ProjectModelResolver resolver) {
        def modelNodeName = 'distributionsGoingBackward'
        def direction = { currentProject -> getProjectBefore(currentProject) }

        return getDistributions(project, resolver, modelNodeName, direction)
    }

    private ModelMap<Distribution> getDistributionsAfter(
            ProjectIdentifier project, ProjectModelResolver resolver) {
        def modelNodeName = 'distributionsGoingForward'
        def direction = { currentProject -> getProjectAfter(currentProject) }

        return getDistributions(project, resolver, modelNodeName, direction)
    }

    private ModelMap<Distribution> getDistributions(
            ProjectIdentifier project, ProjectModelResolver resolver,
            String modelNodeName, Closure direction) {
        def distributions = null
        def projectModel = null
        def currentProject = project
        def nextProject = direction(currentProject)

        while (distributions == null && nextProject != null) {
            currentProject = nextProject

            projectModel = resolver.resolveProjectModel(currentProject.path)

            distributions = projectModel.find(modelNodeName, Object)
            nextProject = direction(currentProject)
        }

        if (distributions == null)
            return null

        def modelNodePath = ModelPath.path(modelNodeName)
        def type = ModelType.of(Object)
        def state = State.Finalized

        return projectModel.atStateOrLater(modelNodePath, type, state)
    }

    private List<Project> getAllProjects(ProjectIdentifier project) {
        def projects = project.rootProject.allprojects

        return projects.sort() { a, b -> a.path <=> b.path }
    }

    private Project getProjectBefore(ProjectIdentifier project) {
        def projects = getAllProjects(project)
        def index = projects.findIndexOf { it.path == project.path }

        if (index > 0)
            return projects[index - 1]
        else
            return null
    }

    private Project getProjectAfter(ProjectIdentifier project) {
        def projects = getAllProjects(project)
        def index = projects.findIndexOf { it.path == project.path }
        def lastIndex = projects.size() - 1

        if (index < lastIndex)
            return projects[index + 1]
        else
            return null
    }

    @Model
    public void allDistributions(ModelMap<Distribution> allDistributions,
            ProjectIdentifier project,
            @Path('distributionsFromProjectsBefore')
                ModelMap<Distribution> distributionsFromBefore,
            @Path('distributions') DistributionContainer currentDistributions,
            @Path('distributionsFromProjectsAfter')
                ModelMap<Distribution> distributionsFromAfter) {
        distributionsFromBefore.keySet().each { key ->
            allDistributions.put(key, distributionsFromBefore.get(key))
        }

        currentDistributions.each { distribution ->
            def name = "$project.path:$distribution.name"

            allDistributions.put(name, distribution)
        }

        distributionsFromAfter.keySet().each { key ->
            allDistributions.put(key, distributionsFromAfter.get(key))
        }
    }

    @Mutate
    public void processUninstallationDependencies(@Each RpmUninstall task,
            @Path('allDistributions') ModelMap<Distribution> allDistributions) {
        allDistributions.each { distribution ->
            def rpmDistribution = new RpmDistribution(distribution)

            boolean shouldDepend = rpmDistribution.dependencies.any {
                    dependency ->
                dependency.uninstallTask == task.path
            }

            if (shouldDepend)
                task.dependsOn rpmDistribution.taskPathFor('uninstallRpm')
        }
    }
}
