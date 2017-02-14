package br.lnls.dig.gradle.nativedistribution.tasks.internal

import org.gradle.api.file.FileCollection
import org.gradle.api.file.FileVisitDetails

import br.lnls.dig.gradle.nativedistribution.tasks.Rpm

class RpmArchiveHeadersAction extends AbstractRpmArchiveAction  {
    FileCollection exportedHeaders

    public RpmArchiveHeadersAction(Rpm rpmTask) {
        super(rpmTask)

        exportedHeaders = rpmTask.exportedHeaders
    }

    @Override
    protected String getPackageName() {
        return projectName + "-devel"
    }

    @Override
    protected void addArchiveFiles() {
        exportedHeaders.asFileTree.visit { fileDetails ->
            if (!fileDetails.isDirectory())
                addHeaderFile(fileDetails)
        }
    }

    private void addHeaderFile(FileVisitDetails header) {
        def headerFilePath = "$installationPrefix/include/$header.relativePath"

        rpmBuilder.addFile(headerFilePath, header.file)
    }
}
