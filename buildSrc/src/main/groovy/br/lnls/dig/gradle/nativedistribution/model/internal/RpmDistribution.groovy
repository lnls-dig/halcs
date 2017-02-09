package br.lnls.dig.gradle.nativedistribution.model.internal

import org.gradle.api.distribution.Distribution
import org.gradle.nativeplatform.BuildType
import org.gradle.nativeplatform.Flavor
import org.gradle.nativeplatform.NativeBinarySpec
import org.gradle.nativeplatform.NativeExecutableBinarySpec
import org.gradle.nativeplatform.SharedLibraryBinarySpec
import org.gradle.platform.base.Platform

import org.redline_rpm.header.Architecture

import br.lnls.dig.gradle.nativedistribution.tasks.internal.RpmDependency

public class RpmDistribution {
    private boolean development
    private Distribution distribution

    RpmDistribution(Distribution distribution) {
        this.distribution = distribution

        development = distribution.usage == 'development'
    }

    String getName() {
        return distribution.name
    }

    String taskPathFor(String suffix) {
        def taskName = distribution.taskNameFor(suffix)

        return "$distribution.project.path:$taskName"
    }

    boolean isDevelopment() {
        return development
    }

    String getOutputFileName() {
        def suffix = distribution.usage == 'development' ? '-devel' : ''
        def arch = architecture.toString().toLowerCase()

        return "$distribution.project.name$suffix-$version-1.${arch}.rpm"
    }

    Set<RpmDependency> getDependencies() {
        def dependencies = new LinkedHashSet<RpmDependency>()

        distribution.dependencies.each { dependency ->
            dependencies.add(new RpmDependency(dependency))
        }

        if (isDevelopment())
            dependencies.add(dependencyToBaseRpm)

        return dependencies
    }

    RpmDependency getDependencyToBaseRpm() {
        def dependency = new RpmDependency(distribution.project.name, version)
        def developmentBuildTaskName = distribution.taskNameFor('distRpm')
        def developmentInstallTaskName = distribution.taskNameFor('installRpm')
        def developmentUninstallTaskName =
                distribution.taskNameFor('uninstallRpm')

        dependency.buildTask = baseRpmTaskName(developmentBuildTaskName)
        dependency.installTask = baseRpmTaskName(developmentInstallTaskName)
        dependency.uninstallTask = baseRpmTaskName(developmentUninstallTaskName)

        return dependency
    }

    private String baseRpmTaskName(String developmentName) {
        int developmentIndex = developmentName.lastIndexOf('Development')
        int developmentEndIndex = developmentIndex + 'Development'.length()

        String firstPart = developmentName.substring(0, developmentIndex)
        String secondPart = developmentName.substring(developmentEndIndex)

        return "$distribution.project.path:$firstPart$secondPart"
    }

    Architecture getArchitecture() {
        String architectureName = distribution.platform.architecture.name

        switch (architectureName) {
            case "x86":
                return Architecture.I386
            case "x86-64":
                return Architecture.X86_64
            default:
                String message = "Unsupported architecture $architecture"
                throw new UnsupportedOperationException(message)
        }
    }

    String getVersion() {
        return distribution.project.version.toString().replaceAll("-", "_")
    }

    BuildType getBuildType() {
        return distribution.buildType
    }

    Flavor getFlavor() {
        return distribution.flavor
    }

    Platform getPlatform() {
        return distribution.platform
    }

    Set<NativeExecutableBinarySpec> getExecutables() {
        return distribution.executables
    }

    Set<SharedLibraryBinarySpec> getSharedLibraries() {
        return distribution.sharedLibraries
    }

    Set<NativeBinarySpec> getBinariesWithSysFiles() {
        return distribution.binariesWithSysFiles
    }
}
