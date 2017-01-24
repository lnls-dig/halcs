package br.lnls.dig.gradle.nativedistribution.model.internal

import org.gradle.api.distribution.internal.DefaultDistribution
import org.gradle.api.file.FileCollection
import org.gradle.api.internal.file.FileOperations
import org.gradle.api.Task
import org.gradle.language.nativeplatform.HeaderExportingSourceSet
import org.gradle.nativeplatform.BuildType
import org.gradle.nativeplatform.Flavor
import org.gradle.nativeplatform.NativeBinarySpec
import org.gradle.nativeplatform.NativeExecutableBinarySpec
import org.gradle.nativeplatform.SharedLibraryBinarySpec
import org.gradle.platform.base.Platform

import br.lnls.dig.gradle.nativedistribution.model.DistributionVariant
import br.lnls.dig.gradle.sysfiles.model.SysFilesSet

public class DefaultDistributionVariant extends DefaultDistribution
        implements DistributionVariant {
    BuildType buildType
    Flavor flavor
    Platform platform
    String usage

    Set<NativeExecutableBinarySpec> executables
    Set<SharedLibraryBinarySpec> sharedLibraries
    Set<NativeBinarySpec> binariesWithSysFiles

    private FileOperations fileOperations

    DefaultDistributionVariant(String name, FileOperations fileOperations) {
        super(name, fileOperations.copySpec())

        executables = new LinkedHashSet<>()
        sharedLibraries = new LinkedHashSet<>()
        binariesWithSysFiles = new LinkedHashSet<>()

        this.fileOperations = fileOperations
    }

    void addBinaries(Iterable<NativeBinarySpec> binaries) {
        def compatibleBinaries = binaries.findAll() { binary ->
            binary.buildType == buildType &&
            binary.flavor == flavor &&
            binary.targetPlatform == platform
        }

        compatibleBinaries.each { binary ->
            if (binary instanceof NativeExecutableBinarySpec)
                executables.add((NativeExecutableBinarySpec)binary)
            if (binary instanceof SharedLibraryBinarySpec)
                sharedLibraries.add((SharedLibraryBinarySpec)binary)
            if (hasSysFilesSet(binary))
                binariesWithSysFiles.add(binary)
        }
    }

    FileCollection getExecutableFiles() {
        def executableFiles = executables.collect { it.executable.file }

        return fileOperations.files(executableFiles)
    }

    FileCollection getSharedLibraryFiles() {
        def libraryFiles = sharedLibraries.collect { it.sharedLibraryFile }

        return fileOperations.files(libraryFiles)
    }

    FileCollection getExportedHeaders() {
        def sourceSets = sharedLibraries.collect { library ->
            library.inputs.withType(HeaderExportingSourceSet)
        }

        def headers = sourceSets.sum().collect { sourceSet ->
            sourceSet.exportedHeaders
        }

        return headers.sum() ?: fileOperations.files()
    }

    FileCollection getSysFiles() {
        def sysFilesSets = binariesWithSysFiles.collect { binary ->
            def sourceSets = binary.inputs.findAll { sourceSet ->
                sourceSet instanceof SysFilesSet
            }

            sourceSets.collect { it.source }.sum()
        }

        return sysFilesSets.sum() ?: fileOperations.files()
    }

    boolean hasSysFilesSet(NativeBinarySpec binary) {
        def sysFilesSets = binary.inputs.findAll { it instanceof SysFilesSet }

        return !sysFilesSets.isEmpty()
    }

    @Override
    public Set<Task> getBuildTasks() {
        return executables.collect { it.buildTask }
            + sharedLibraries.collect { it.buildTask }
    }
}
