package br.lnls.dig.gradle.nativedistribution.tasks.internal

import org.gradle.api.distribution.Distribution
import org.gradle.api.file.FileCollection
import org.gradle.api.internal.file.copy.CopyAction
import org.gradle.api.internal.file.copy.CopyActionProcessingStream
import org.gradle.api.internal.file.copy.FileCopyDetailsInternal
import org.gradle.api.internal.file.CopyActionProcessingStreamAction
import org.gradle.api.internal.tasks.SimpleWorkResult
import org.gradle.api.tasks.WorkResult

import org.redline_rpm.Builder
import org.redline_rpm.header.Architecture
import org.redline_rpm.header.Os

import br.lnls.dig.gradle.nativedistribution.tasks.Rpm

abstract class AbstractRpmArchiveAction implements CopyAction {
    private File gatheredFilesDirectory
    private File outputDirectory

    Builder rpmBuilder
    Distribution distribution
    FileCollection executables
    String installationPrefix
    String projectName
    String projectVersion
    Set<Dependency> dependencies

    public AbstractRpmArchiveAction(Rpm rpmTask) {
        distribution = rpmTask.distribution
        executables = rpmTask.executables
        installationPrefix = rpmTask.installationPrefix
        outputDirectory = rpmTask.outputDirectory
        projectName = rpmTask.project.name
        projectVersion = rpmTask.project.version
        dependencies = rpmTask.dependencies

        gatheredFilesDirectory = new File(rpmTask.buildDir,
                "tmp/distributions/${distribution.name}DistRpm")

        rpmBuilder = new Builder()
        rpmBuilder.setPackage(packageName, version, "1")
        rpmBuilder.setPlatform(architecture, operatingSystem)
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

    Os getOperatingSystem() {
        return Os.LINUX
    }

    String getVersion() {
        return projectVersion.replaceAll("-", "_")
    }

    @Override
    public WorkResult execute(CopyActionProcessingStream stream) {
        stream.process(new RpmArchiveInternalAction())

        addArchiveFiles()
        addDependencies()

        rpmBuilder.build(outputDirectory)

        return new SimpleWorkResult(true)
    }

    protected void addDependencies() {
        dependencies.each { dependency ->
            def name = dependency.name
            def version = dependency.version
            def versionFlag = dependency.versionFlag

            rpmBuilder.addDependency(name, version, versionFlag)
        }
    }

    protected void addArchiveFiles() {
        addArchiveExecutableFiles()
        addArchiveLibraryFiles()
    }

    protected void addArchiveExecutableFiles() {
        executables.each { executableFile -> addExecutableFile(executableFile) }
    }

    protected void addExecutableFile(File executableFile) {
        def executableFilePath = "$installationPrefix/bin/$executableFile.name"
        def permissions = 0755

        rpmBuilder.addFile(executableFilePath, executableFile, permissions)
    }

    protected abstract void addArchiveLibraryFiles()
    protected abstract String getPackageName()

    private class RpmArchiveInternalAction
            implements CopyActionProcessingStreamAction {
        @Override
        public void processFile(FileCopyDetailsInternal details) {
            if (!details.isDirectory())
                visitFile(details)
        }

        private void visitFile(FileCopyDetailsInternal details) {
            File target = new File(gatheredFilesDirectory, details.path)

            details.copyTo(target)

            rpmBuilder.addFile("$installationPrefix/$details.path", target)
        }
    }
}
