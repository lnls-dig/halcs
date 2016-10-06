package br.lnls.dig.gradle.nativedistribution.model.internal

import org.gradle.api.distribution.internal.DefaultDistribution
import org.gradle.api.file.CopySpec
import org.gradle.api.Task
import org.gradle.nativeplatform.BuildType
import org.gradle.nativeplatform.Flavor
import org.gradle.nativeplatform.NativeExecutableBinarySpec
import org.gradle.nativeplatform.SharedLibraryBinarySpec
import org.gradle.platform.base.Platform

import br.lnls.dig.gradle.nativedistribution.model.DistributionVariant

public class DefaultDistributionVariant extends DefaultDistribution
        implements DistributionVariant {
    BuildType buildType
    Flavor flavor
    Platform platform
    String usage

    Set<NativeExecutableBinarySpec> executables
    Set<SharedLibraryBinarySpec> sharedLibraries

    DefaultDistributionVariant(String name, CopySpec spec) {
        super(name, spec)

        executables = new LinkedHashSet<>()
        sharedLibraries = new LinkedHashSet<>()
    }

    void addExecutablesFrom(Iterable<NativeExecutableBinarySpec> executables) {
        this.executables += executables.findAll() { executable ->
            executable.buildType == buildType &&
            executable.flavor == flavor &&
            executable.targetPlatform == platform
        }
    }

    void addSharedLibrariesFrom(
            Iterable<SharedLibraryBinarySpec> sharedLibraries) {
        this.sharedLibraries += sharedLibraries.findAll() { sharedLibrary ->
            sharedLibrary.buildType == buildType &&
            sharedLibrary.flavor == flavor &&
            sharedLibrary.targetPlatform == platform
        }
    }

    @Override
    public Set<Task> getBuildTasks() {
        return executables.collect { it.buildTask }
            + sharedLibraries.collect { it.buildTask }
    }
}
