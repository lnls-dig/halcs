package br.lnls.dig.gradle.nativedistribution.tasks.internal

import br.lnls.dig.gradle.nativedistribution.tasks.Rpm

class RpmArchiveSharedLibrariesAction extends AbstractRpmArchiveAction {
    Set<File> sharedLibraries
    Set<File> unpackagedDependencies

    public RpmArchiveSharedLibrariesAction(Rpm rpmTask) {
        super(rpmTask)

        sharedLibraries = rpmTask.sharedLibraries.findAll { it.exists() }
        unpackagedDependencies = rpmTask.unpackagedDependencies
                .findAll { it.exists() }
    }

    @Override
    protected String getPackageName() {
        return projectName
    }

    @Override
    protected void addArchiveLibraryFiles() {
        String libraryVersion = version.replaceAll("[^0-9.].*", "")
        String[] libraryVersionParts = libraryVersion.split("\\.")

        sharedLibraries.each { sharedLibrary ->
            addSharedLibraryFile(sharedLibrary, libraryVersion)
            addSharedLibraryLinks(sharedLibrary, libraryVersionParts)
        }
        unpackagedDependencies.each { sharedLibrary ->
            addSharedLibraryFile(sharedLibrary, libraryVersion)
            addSharedLibraryLinks(sharedLibrary, libraryVersionParts)
        }
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

    private void addSharedLibraryFile(File library, String libraryVersion) {
        String libraryFileName = "${library.name}.$libraryVersion"
        String libraryFilePath = "$installationPrefix/lib/$libraryFileName"
        int permissions = 0755

        rpmBuilder.addFile(libraryFilePath, library, permissions)

    }

    private void addSharedLibraryLink(File library, String sourceVersion,
            String targetVersion) {
        String linkFileName = library.name + sourceVersion
        String linkFilePath = "$installationPrefix/lib/$linkFileName"
        String linkTarget = "$library.name$targetVersion"
        int linkFilePermissions = 0755

        rpmBuilder.addLink(linkFilePath, linkTarget, linkFilePermissions)
    }
}
