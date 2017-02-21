package br.lnls.dig.gradle.nativeinstall.plugins

import org.gradle.api.distribution.Distribution
import org.gradle.api.internal.resolve.ProjectModelResolver
import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.api.Task
import org.gradle.internal.service.ServiceRegistry
import org.gradle.model.Each
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.RuleSource

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.plugins.NativeDistributionPlugin
import br.lnls.dig.gradle.nativeinstall.tasks.NativeInstall

import static org.gradle.api.distribution.plugins.DistributionPlugin.MAIN_DISTRIBUTION_NAME

class NativeInstallPlugin implements Plugin<Project> {
    void apply(Project project) {
        project.pluginManager.apply(NativeDistributionPlugin)
    }

    public static class Rules extends RuleSource {
        @Mutate
        public void addNativeInstallTasks(ModelMap<Task> tasks,
                DistributionContainer distributions) {
            distributions.all { distribution ->
                def taskName = getTaskNameFor(distribution, "Install")

                tasks.create(taskName, NativeInstall) { task ->
                    task.distribution = distribution
                    task.dependsOn distribution.buildTasks
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
        public void processDetectedDependencies(@Each NativeInstall installTask,
                ServiceRegistry serviceRegistry) {
            def projectModelResolver = serviceRegistry.get(ProjectModelResolver)

            installTask.resolveDependencies(projectModelResolver)
        }

        @Mutate
        public void addDependencyBetweenDevelopmentAndRuntimeInstallation(
                @Each NativeInstall installTask) {
            if (installTask.distribution.usage == 'development') {
                def developmentInstallTask = installTask
                def runtimeInstallTask = getRuntimeInstallTaskFor(installTask)

                developmentInstallTask.dependsOn runtimeInstallTask
            }
        }

        private String getRuntimeInstallTaskFor(
                NativeInstall developmentInstallTask) {
            String developmentName = developmentInstallTask.name

            int developmentIndex = developmentName.lastIndexOf('Development')
            int developmentEndIndex = developmentIndex + 'Development'.length()

            String firstPart = developmentName.substring(0, developmentIndex)
            String secondPart = developmentName.substring(developmentEndIndex)

            return firstPart + secondPart
        }
    }
}
