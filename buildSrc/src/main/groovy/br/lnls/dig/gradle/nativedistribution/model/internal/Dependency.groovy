package br.lnls.dig.gradle.nativedistribution.model.internal

import org.gradle.api.distribution.Distribution

class Dependency implements Serializable {
    Distribution distribution
    String projectPath
    String projectVersion

    Dependency(String projectPath, String projectVersion,
            Distribution distribution) {
        this.projectPath = projectPath
        this.projectVersion = projectVersion
        this.distribution = distribution
    }

    String getProjectName() {
        return projectPath.split(':').last()
    }

    boolean equals(Object other) {
        if (!other instanceof Dependency)
            return false

        return projectPath.equals(other.projectPath) &&
            projectVersion.equals(other.projectVersion)
    }

    int hashCode() {
        return projectPath.hashCode()
    }
}
