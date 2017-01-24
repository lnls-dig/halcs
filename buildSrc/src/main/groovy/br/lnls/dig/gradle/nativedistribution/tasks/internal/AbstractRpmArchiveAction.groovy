package br.lnls.dig.gradle.nativedistribution.tasks.internal

import java.nio.file.attribute.PosixFilePermission
import java.nio.file.Files

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

import br.lnls.dig.gradle.nativedistribution.tasks.Rpm

abstract class AbstractRpmArchiveAction implements CopyAction {
    private File gatheredFilesDirectory
    private File outputFile

    Builder rpmBuilder
    Distribution distribution
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
        version = rpmTask.version
        dependencies = rpmTask.dependencies

        gatheredFilesDirectory = new File(rpmTask.buildDir,
                "tmp/distributions/${distribution.name}DistRpm")

        rpmBuilder = new Builder()
        rpmBuilder.setPackage(packageName, version, "1")
        rpmBuilder.setPlatform(rpmTask.architecture, operatingSystem)
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

    protected void addArchiveFiles() {
        addArchiveExecutableFiles()
        addArchiveLibraryFiles()
        addArchiveSysFiles()
    }

    protected void addArchiveExecutableFiles() {
        executables.each { executableFile -> addExecutableFile(executableFile) }
    }

    protected void addExecutableFile(File executableFile) {
        def executableFilePath = "$installationPrefix/bin/$executableFile.name"
        def permissions = 0755

        rpmBuilder.addFile(executableFilePath, executableFile, permissions)
    }

    protected void addArchiveSysFiles() {
        sysFiles.asFileTree.visit { visit ->
            if (!visit.isDirectory())
                addSysFile(visit.file, visit.relativePath.toString())
        }
    }

    protected void addSysFile(File sysFile, String path) {
        def sysFilePath = "/$path"
        def permissions = getFilePermissions(sysFile)

        rpmBuilder.addFile(sysFilePath, sysFile, permissions)
    }

    protected int getFilePermissions(File file) {
        def permissionSet = Files.getPosixFilePermissions(file.toPath())
        int permissions = 0

        permissionSet.each { permission ->
            switch (permission) {
                case PosixFilePermission.OTHERS_EXECUTE:
                    permissions |= 01;
                    break;
                case PosixFilePermission.OTHERS_WRITE:
                    permissions |= 02;
                    break;
                case PosixFilePermission.OTHERS_READ:
                    permissions |= 04;
                    break;
                case PosixFilePermission.GROUP_EXECUTE:
                    permissions |= 010;
                    break;
                case PosixFilePermission.GROUP_WRITE:
                    permissions |= 020;
                    break;
                case PosixFilePermission.GROUP_READ:
                    permissions |= 040;
                    break;
                case PosixFilePermission.OWNER_EXECUTE:
                    permissions |= 0100;
                    break;
                case PosixFilePermission.OWNER_WRITE:
                    permissions |= 0200;
                    break;
                case PosixFilePermission.OWNER_READ:
                    permissions |= 0400;
                    break;
            }
        }

        return permissions
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
