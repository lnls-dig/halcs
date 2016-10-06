package br.lnls.dig.gradle.nativedistribution.model

import org.gradle.api.distribution.Distribution
import org.gradle.api.Task
import org.gradle.nativeplatform.BuildType
import org.gradle.nativeplatform.Flavor
import org.gradle.nativeplatform.NativeExecutableBinarySpec
import org.gradle.nativeplatform.SharedLibraryBinarySpec
import org.gradle.platform.base.Platform

public interface DistributionVariant extends Distribution {
    BuildType getBuildType()
    Flavor getFlavor()
    Platform getPlatform()
    String getUsage()

    void setBuildType(BuildType buildType)
    void setFlavor(Flavor flavor)
    void setPlatform(Platform platform)
    void setUsage(String usage)

    void addExecutablesFrom(Iterable<NativeExecutableBinarySpec> executables)
    void addSharedLibrariesFrom(
            Iterable<SharedLibraryBinarySpec> sharedLibraries)

    Set<Task> getBuildTasks()
}
