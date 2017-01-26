package br.lnls.dig.gradle.nativedistribution.tasks.internal

import java.nio.file.attribute.PosixFilePermission
import java.nio.file.Files

import br.lnls.dig.gradle.nativedistribution.tasks.Rpm

class RpmArchiveSharedLibrariesAction extends AbstractRpmArchiveAction {
    Set<File> sharedLibraries

    public RpmArchiveSharedLibrariesAction(Rpm rpmTask) {
        super(rpmTask)

        sharedLibraries = rpmTask.sharedLibraries.findAll { it.exists() }
    }

    @Override
    protected String getPackageName() {
        return projectName
    }

    @Override
    protected void addArchiveFiles() {
        addExecutables()
        addLibraries()
        addSysFiles()
    }

    protected void addExecutables() {
        executables.each { executableFile -> addExecutableFile(executableFile) }
    }

    protected void addExecutableFile(File executableFile) {
        def executableFilePath = "$installationPrefix/bin/$executableFile.name"
        def permissions = 0755

        rpmBuilder.addFile(executableFilePath, executableFile, permissions)
    }

    protected void addLibraries() {
        String libraryVersion = version.replaceAll("[^0-9.].*", "")
        String[] libraryVersionParts = libraryVersion.split("\\.")

        sharedLibraries.each { sharedLibrary ->
            addSharedLibraryFile(sharedLibrary, libraryVersion)
            addSharedLibraryLinks(sharedLibrary, libraryVersionParts)
        }
    }

    private void addSharedLibraryFile(File library, String libraryVersion) {
        String libraryFileName = "${library.name}.$libraryVersion"
        String libraryFilePath = "$installationPrefix/lib/$libraryFileName"
        int permissions = 0755

        rpmBuilder.addFile(libraryFilePath, library, permissions)

    }

    private void addSharedLibraryLinks(File library,
            String[] versionNumbers) {
        int versionNumbersCount = versionNumbers.length

        StringBuilder currentVersionSuffix = new StringBuilder()
        String sourceVersion = ""

        for (versionPart in (0..<versionNumbersCount)) {
            currentVersionSuffix.append('.')
            currentVersionSuffix.append(versionNumbers[versionPart])

            String targetVersion = currentVersionSuffix.toString()

            addSharedLibraryLink(library, sourceVersion, targetVersion)

            sourceVersion = targetVersion
        }
    }

    private void addSharedLibraryLink(File library, String sourceVersion,
            String targetVersion) {
        String linkFileName = library.name + sourceVersion
        String linkFilePath = "$installationPrefix/lib/$linkFileName"
        String linkTarget = "$library.name$targetVersion"
        int linkFilePermissions = 0755

        rpmBuilder.addLink(linkFilePath, linkTarget, linkFilePermissions)
    }

    protected void addSysFiles() {
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
}
