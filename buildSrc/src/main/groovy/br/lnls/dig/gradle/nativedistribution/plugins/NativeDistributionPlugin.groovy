package br.lnls.dig.gradle.nativedistribution.plugins

import org.gradle.api.distribution.Distribution
import org.gradle.api.internal.resolve.ProjectModelResolver
import org.gradle.api.Plugin
import org.gradle.api.plugins.ExtensionContainer
import org.gradle.api.Project
import org.gradle.api.Task
import org.gradle.internal.service.ServiceRegistry
import org.gradle.model.Each
import org.gradle.model.Finalize
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource
import org.gradle.nativeplatform.BuildType
import org.gradle.nativeplatform.BuildTypeContainer
import org.gradle.nativeplatform.Flavor
import org.gradle.nativeplatform.FlavorContainer
import org.gradle.nativeplatform.NativeBinarySpec
import org.gradle.nativeplatform.NativeLibrarySpec
import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform
import org.gradle.nativeplatform.TargetedNativeComponent
import org.gradle.platform.base.Platform
import org.gradle.platform.base.PlatformContainer

import br.lnls.dig.gradle.distribution.plugins.DistributionPlugin
import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.model.DistributionVariant
import br.lnls.dig.gradle.nativedistribution.plugins.internal.DistributionVariants
import br.lnls.dig.gradle.nativedistribution.tasks.Rpm

import static org.gradle.api.distribution.plugins.DistributionPlugin.MAIN_DISTRIBUTION_NAME

class NativeDistributionPlugin implements Plugin<Project> {
    void apply(Project project) {
        project.pluginManager.apply(DistributionPlugin)
    }

    public static class Rules extends RuleSource {
        @Finalize
        public void addCurrentPlatformIfNoPlatformsWereDefined(
                PlatformContainer platforms) {
            if (platforms.isEmpty())
                platforms.add(new DefaultNativePlatform("current"))
        }

        @Mutate
        public void setTargetPlatformOfNativeComponentsIfNecessary(
                @Each TargetedNativeComponent component) {
            if (component.targetPlatforms.isEmpty())
                component.targetPlatform 'current'
        }

        @Mutate
        public void addDistributionsForBuildVariations(
                DistributionContainer distributions,
                BuildTypeContainer buildTypes, PlatformContainer platforms,
                FlavorContainer flavors,
                ModelMap<NativeLibrarySpec> nativeLibraries) {
            def hasLibraries = !nativeLibraries.isEmpty()
            def usages = getUsageVariations(hasLibraries)

            removeMainDistribution(distributions)

            DistributionVariants.from(buildTypes, platforms, flavors, usages)
                .addTo(distributions)
        }

        private String[] getUsageVariations(boolean hasLibraries) {
            if (hasLibraries)
                return ["", "development"].toArray()
            else
                return [""].toArray()
        }

        private void removeMainDistribution(
                DistributionContainer distributions) {
            distributions.removeAll {
                it.name == MAIN_DISTRIBUTION_NAME
            }
        }

        @Mutate
        public void addNativeBinariesToDistribution(
                DistributionContainer distributions,
                @Path("binaries") ModelMap<NativeBinarySpec> binaries) {
            distributions.all { distribution ->
                distribution.addBinaries(binaries)
            }
        }

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

        private String getRpmTaskWithLibrariesFor(Rpm developmentRpmTask) {
            String developmentName = developmentRpmTask.name

            int developmentIndex = developmentName.lastIndexOf('Development')
            int developmentEndIndex = developmentIndex + 'Development'.length()

            String firstPart = developmentName.substring(0, developmentIndex)
            String secondPart = developmentName.substring(developmentEndIndex)

            return firstPart + secondPart
        }
    }
}
