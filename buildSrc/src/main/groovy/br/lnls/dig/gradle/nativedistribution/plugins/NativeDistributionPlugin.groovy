package br.lnls.dig.gradle.nativedistribution.plugins

import org.gradle.api.distribution.Distribution
import org.gradle.api.internal.resolve.ProjectModelResolver
import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.api.Task
import org.gradle.internal.service.ServiceRegistry
import org.gradle.model.Each
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource

import br.lnls.dig.gradle.distribution.plugins.DistributionPlugin
import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.plugins.CurrentPlatformPlugin
import br.lnls.dig.gradle.nativedistribution.plugins.DefaultNativeDistributionsPlugin
import br.lnls.dig.gradle.nativedistribution.tasks.Rpm
import br.lnls.dig.gradle.nativedistribution.tasks.RpmInstall

import static org.gradle.api.distribution.plugins.DistributionPlugin.MAIN_DISTRIBUTION_NAME

class NativeDistributionPlugin implements Plugin<Project> {
    void apply(Project project) {
        project.pluginManager.apply(DistributionPlugin)
        project.pluginManager.apply(CurrentPlatformPlugin)
        project.pluginManager.apply(DefaultNativeDistributionsPlugin)
    }

    public static class Rules extends RuleSource {
        @Mutate
        public void addNativeDistributionTasks(ModelMap<Task> tasks,
                DistributionContainer distributions,
                @Path("buildDir") File buildDir) {
            distributions.all { distribution ->
                tasks.create(getTaskNameFor(distribution, "distRpm"), Rpm) {
                        task ->
                    task.distribution = distribution
                    task.dependsOn(distribution.buildTasks)
                    task.buildDir = buildDir
                    task.outputDirectory = new File(buildDir,
                            "/distributions/$task.distribution.name")
                }
            }
        }

        private String getTaskNameFor(Distribution distribution,
                String taskSuffix) {
            if (distribution.name == MAIN_DISTRIBUTION_NAME)
                return taskSuffix
            else
                return distribution.name + taskSuffix.capitalize()
        }

        @Mutate
        public void processDetectedDependencies(@Each Rpm rpmTask,
                ServiceRegistry serviceRegistry) {
            def projectModelResolver = serviceRegistry.get(ProjectModelResolver)

            rpmTask.resolveDependencies(projectModelResolver)
        }

        @Mutate
        public void addDependencyBetweenDevelopmentAndRuntimeRpms(
                @Each Rpm rpmTask) {
            if (rpmTask.distribution.usage == 'development') {
                rpmTask.dependsOn getRpmTaskWithLibrariesFor(rpmTask)
                rpmTask.addDependency(rpmTask.project.name,
                        rpmTask.project.version.toString())
            }
        }

        private String getRpmTaskWithLibrariesFor(Task developmentRpmTask) {
            String developmentName = developmentRpmTask.name

            int developmentIndex = developmentName.lastIndexOf('Development')
            int developmentEndIndex = developmentIndex + 'Development'.length()

            String firstPart = developmentName.substring(0, developmentIndex)
            String secondPart = developmentName.substring(developmentEndIndex)

            return firstPart + secondPart
        }

        @Mutate
        public void addInstallationTasks(ModelMap<Task> tasks,
                DistributionContainer distributions,
                @Path("buildDir") File buildDir) {
            distributions.all { distribution ->
                tasks.create(getTaskNameFor(distribution, "installRpm"),
                        RpmInstall) { task ->
                    task.dependsOn getTaskNameFor(distribution, "distRpm")
                    task.distribution = distribution
                    task.rpmDirectory = new File(buildDir,
                            "/distributions/$task.distribution.name")
                }
            }
        }

        @Mutate
        public void addDependencyBetweenDevelopmentAndRuntimeRpmInstallations(
                @Each RpmInstall task) {
            if (task.distribution.usage == 'development')
                task.dependsOn getRpmTaskWithLibrariesFor(task)
        }

        @Mutate
        public void processDetectedInstallationDependencies(
                @Each RpmInstall task, ServiceRegistry serviceRegistry) {
            def projectModelResolver = serviceRegistry.get(ProjectModelResolver)

            task.resolveDependencies(projectModelResolver)
        }
    }
}
