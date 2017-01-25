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
import org.redline_rpm.header.Os

import br.lnls.dig.gradle.nativedistribution.model.internal.RpmDistribution
import br.lnls.dig.gradle.nativedistribution.tasks.Rpm

abstract class AbstractRpmArchiveAction implements CopyAction {
    private File gatheredFilesDirectory
    private File outputFile

    Builder rpmBuilder
    RpmDistribution distribution
    FileCollection executables
    FileCollection sysFiles
    String installationPrefix
    String projectName
    String version
    Set<Dependency> dependencies

    public AbstractRpmArchiveAction(Rpm rpmTask) {
        distribution = rpmTask.distribution
        executables = rpmTask.executables
        sysFiles = rpmTask.sysFiles
        installationPrefix = rpmTask.installationPrefix
        outputFile = rpmTask.outputFile
        projectName = rpmTask.project.name
        version = distribution.version
        dependencies = rpmTask.dependencies

        gatheredFilesDirectory = new File(rpmTask.buildDir,
                "tmp/distributions/${distribution.name}DistRpm")

        rpmBuilder = new Builder()
        rpmBuilder.setPackage(packageName, distribution.version, "1")
        rpmBuilder.setPlatform(distribution.architecture, operatingSystem)
    }

    Os getOperatingSystem() {
        return Os.LINUX
    }

    @Override
    public WorkResult execute(CopyActionProcessingStream stream) {
        stream.process(new RpmArchiveInternalAction())

        addArchiveFiles()
        addDependencies()

        buildRpmFile()

        return new SimpleWorkResult(true)
    }

    protected void buildRpmFile() {
        RandomAccessFile rpmFile = new RandomAccessFile(outputFile, "rw")

        rpmBuilder.build(rpmFile.channel)
    }

    protected void addDependencies() {
        dependencies.each { dependency ->
            def name = dependency.name
            def version = dependency.version.replaceAll("-", "_")
            def versionFlag = dependency.versionFlag

            rpmBuilder.addDependency(name, version, versionFlag)
        }
    }

    protected abstract void addArchiveFiles()
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
