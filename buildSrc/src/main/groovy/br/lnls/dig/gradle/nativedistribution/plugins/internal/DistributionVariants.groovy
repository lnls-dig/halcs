package br.lnls.dig.gradle.nativedistribution.plugins.internal

import org.gradle.api.internal.project.ProjectIdentifier
import org.gradle.api.NamedDomainObjectCollection
import org.gradle.nativeplatform.BuildType
import org.gradle.nativeplatform.BuildTypeContainer
import org.gradle.nativeplatform.Flavor
import org.gradle.nativeplatform.FlavorContainer
import org.gradle.nativeplatform.SharedLibraryBinarySpec
import org.gradle.platform.base.Platform
import org.gradle.platform.base.PlatformContainer

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.model.DistributionVariant
import br.lnls.dig.gradle.nativedistribution.model.internal.DefaultDistributionVariant

public class DistributionVariants {
    public static from(BuildTypeContainer buildTypes,
            PlatformContainer platforms, FlavorContainer flavors,
            String[] usages) {
        return new DistributionVariants(buildTypes, platforms, flavors, usages)
    }

    private BuildTypeContainer buildTypes
    private PlatformContainer platforms
    private FlavorContainer flavors
    private String[] usages

    private BuildType mainBuildType
    private Platform mainPlatform

    private DistributionVariants(BuildTypeContainer buildTypes,
            PlatformContainer platforms, FlavorContainer flavors,
            String[] usages) {
        this.buildTypes = buildTypes
        this.platforms = platforms
        this.flavors = flavors
        this.usages = usages;

        findMainBuildType()
        findMainPlatform()
    }

    private void findMainBuildType() {
        mainBuildType = getSoleElementFrom(buildTypes) ?:
            buildTypes.findByName('release') ?:
            buildTypes.findByName('production') ?:
            null
    }

    private void findMainPlatform() {
        mainPlatform = getSoleElementFrom(platforms) ?:
            platforms.findByName('current') ?:
            null
    }

    private <T> T getSoleElementFrom(NamedDomainObjectCollection<T> container) {
        if (container.size() == 1)
            return container.first()
        else
            return null
    }


    public void addTo(DistributionContainer distributions,
            ProjectIdentifier project) {
        def type = DistributionVariant
        def implementation = DefaultDistributionVariant
        def variations = createAllVariations()

        distributions.registerFactory(type, implementation)

        variations.each { variation ->
            distributions.create(variation.name, type) { distribution ->
                distribution.project = project
                distribution.flavor = variation.flavor
                distribution.platform = variation.platform
                distribution.buildType = variation.buildType
                distribution.usage = variation.usage
            }
        }
    }

    private class Variation {
        BuildType buildType
        Platform platform
        Flavor flavor
        String usage

        String getName() {
            String buildTypeName = buildType.name
            String platformName = platform.name

            if (buildType == mainBuildType)
                buildTypeName = ""

            if (platform == mainPlatform)
                platformName = ""

            return flavor.name + platformName.capitalize() +
                    buildTypeName.capitalize() + usage.capitalize()
        }
    }

    private List<Variation> createAllVariations() {
        def validFlavors = flavors.isEmpty() ? [null] : flavors
        def combinations =
                [buildTypes, platforms, validFlavors, usages].combinations()

        return combinations.collect { combination ->
            def variation = new Variation()

            variation.buildType = combination[0]
            variation.platform = combination[1]
            variation.flavor = combination[2]
            variation.usage = combination[3]

            return variation
        }
    }
}
