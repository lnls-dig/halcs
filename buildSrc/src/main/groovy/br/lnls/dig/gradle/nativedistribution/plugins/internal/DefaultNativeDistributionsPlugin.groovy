package br.lnls.dig.gradle.nativedistribution.plugins

import org.gradle.api.distribution.Distribution
import org.gradle.api.internal.resolve.ProjectModelResolver
import org.gradle.internal.service.ServiceRegistry
import org.gradle.model.Each
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource
import org.gradle.nativeplatform.BuildTypeContainer
import org.gradle.nativeplatform.FlavorContainer
import org.gradle.nativeplatform.NativeBinarySpec
import org.gradle.nativeplatform.NativeLibrarySpec
import org.gradle.platform.base.PlatformContainer

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.plugins.internal.DistributionVariants

import static org.gradle.api.distribution.plugins.DistributionPlugin.MAIN_DISTRIBUTION_NAME

class DefaultNativeDistributionsPlugin extends RuleSource {
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
    public void resolveDistributionDependencies(
            DistributionContainer distributions,
            ServiceRegistry serviceRegistry) {
        def projectModelResolver = serviceRegistry.get(ProjectModelResolver)

        distributions.all { distribution ->
            distribution.resolveDependencies(projectModelResolver)
        }
    }
}
