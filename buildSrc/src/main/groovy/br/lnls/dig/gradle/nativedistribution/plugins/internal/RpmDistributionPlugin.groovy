package br.lnls.dig.gradle.nativedistribution.plugins

import org.gradle.api.Task
import org.gradle.model.Each
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.tasks.Rpm
import br.lnls.dig.gradle.nativedistribution.tasks.RpmInstall

class RpmDistributionPlugin extends RuleSource {
    @Mutate
    public void addNativeDistributionTasks(ModelMap<Task> tasks,
            DistributionContainer distributions,
            @Path("buildDir") File buildDir) {
        distributions.all { distribution ->
            tasks.create(distribution.taskNameFor("distRpm"), Rpm) {
                    task ->
                task.distribution = distribution
                task.dependsOn(distribution.buildTasks)
                task.buildDir = buildDir
                task.outputDirectory = new File(buildDir,
                        "/distributions/$distribution.name")
            }
        }
    }

    @Mutate
    public void addDependencyBetweenDevelopmentAndRuntimeRpms(
            @Each Rpm rpmTask) {
        if (rpmTask.distribution.isDevelopment()) {
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
            tasks.create(distribution.taskNameFor("installRpm"),
                    RpmInstall) { task ->
                task.dependsOn distribution.taskNameFor("distRpm")
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
}
