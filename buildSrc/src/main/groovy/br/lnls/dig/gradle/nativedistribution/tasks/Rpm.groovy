package br.lnls.dig.gradle.nativedistribution.tasks

import org.gradle.api.distribution.Distribution
import org.gradle.api.file.FileCollection
import org.gradle.api.internal.file.copy.CopyAction
import org.gradle.api.internal.resolve.ProjectModelResolver
import org.gradle.api.tasks.bundling.AbstractArchiveTask
import org.gradle.api.tasks.Input
import org.gradle.api.tasks.InputFiles
import org.gradle.api.tasks.OutputFile
import org.gradle.language.nativeplatform.DependentSourceSet
import org.gradle.language.nativeplatform.HeaderExportingSourceSet
import org.gradle.model.internal.registry.ModelRegistry
import org.gradle.nativeplatform.NativeBinarySpec
import org.gradle.nativeplatform.NativeLibrarySpec
import org.gradle.nativeplatform.SharedLibraryBinarySpec

import br.lnls.dig.gradle.nativedistribution.model.internal.RpmDistribution
import br.lnls.dig.gradle.nativedistribution.tasks.internal.RpmDependency
import br.lnls.dig.gradle.nativedistribution.tasks.internal.RpmArchiveHeadersAction
import br.lnls.dig.gradle.nativedistribution.tasks.internal.RpmArchiveSharedLibrariesAction
import br.lnls.dig.gradle.sysfiles.model.SysFilesSet

class Rpm extends AbstractArchiveTask {
    RpmDistribution distribution
    File buildDir
    String installationPrefix

    @Input
    LinkedHashSet<RpmDependency> dependencies

    private File outputDirectory

    FileCollection executables
    FileCollection exportedHeaders
    FileCollection sharedLibraries
    FileCollection sysFiles

    public Rpm() {
        extension = 'rpm'
        installationPrefix = '/usr/local'

        dependencies = new LinkedHashSet<>()
    }

    @OutputFile
    public File getOutputFile() {
        return new File(outputDirectory, distribution.outputFileName)
    }

    public void setOutputDirectory(File outputDirectory) {
        this.outputDirectory = outputDirectory
    }

    @Override @InputFiles
    public FileCollection getSource() {
        def source = super.getSource()

        if (distribution.isDevelopment())
            source += exportedHeaders
        else
            source += sharedLibraries + executables + sysFiles

        return source
    }

    @Override
    protected CopyAction createCopyAction() {
        if (distribution.isDevelopment())
            return new RpmArchiveHeadersAction(this)
        else
            return new RpmArchiveSharedLibrariesAction(this)
    }

    public void addDependencies(Iterable<RpmDependency> newDependencies) {
        newDependencies.each { dependency ->
            dependencies.add(dependency)

            if (dependency.buildTask != null)
                dependsOn dependency.buildTask
        }
    }

    public void addDependency(String name, String version) {
        dependencies.add(new RpmDependency(name, version))
    }

    public void setDistribution(Distribution distribution) {
        this.distribution = new RpmDistribution(distribution, project)

        executables = distribution.executableFiles
        exportedHeaders = distribution.exportedHeaders
        sharedLibraries = distribution.sharedLibraryFiles
        sysFiles = distribution.sysFiles

        addDependencies(this.distribution.dependencies)
    }
}
