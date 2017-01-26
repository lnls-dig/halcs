package br.lnls.dig.gradle.nativedistribution.tasks.internal

import br.lnls.dig.gradle.nativedistribution.model.internal.Dependency

import static org.redline_rpm.header.Flags.EQUAL

class RpmDependency implements Serializable {
    String name
    String version
    int versionFlag

    RpmDependency(String name, String version) {
        this.name = name
        this.version = version
        this.versionFlag = EQUAL
    }

    RpmDependency(Dependency dependency) {
        name = dependency.projectName
        version = dependency.projectVersion
        versionFlag = EQUAL

        if (dependency.distribution.usage == 'development')
            name += '-devel'
    }

    boolean equals(Object other) {
        if (!other instanceof RpmDependency)
            return false

        return name.equals(other.name) &&
            version.equals(other.version) &&
            versionFlag == other.versionFlag
    }

    int hashCode() {
        return name.hashCode()
    }
}
