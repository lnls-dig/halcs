package br.lnls.dig.gradle.nativedistribution.tasks.internal

import static org.redline_rpm.header.Flags.EQUAL

class Dependency implements Serializable {
    String name
    String version
    int versionFlag

    Dependency(String name, String version) {
        this.name = name
        this.version = version
        this.versionFlag = EQUAL
    }
}
