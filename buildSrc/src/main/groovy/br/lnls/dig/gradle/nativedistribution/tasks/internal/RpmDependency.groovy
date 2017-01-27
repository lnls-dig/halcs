package br.lnls.dig.gradle.nativedistribution.tasks.internal

import org.gradle.api.Task

import br.lnls.dig.gradle.nativedistribution.model.internal.Dependency

import static org.redline_rpm.header.Flags.EQUAL

class RpmDependency implements Serializable {
    String name
    String version
    int versionFlag

    String buildTask
    String installTask

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

        buildTask = discoverTask(dependency, 'distRpm')
        installTask = discoverTask(dependency, 'installRpm')
    }

    private String discoverTask(Dependency dependency, String taskSuffix) {
        def taskName = dependency.distribution.taskNameFor(taskSuffix)
        def task = dependency.projectModel.find("tasks.$taskName", Task)

        return task?.path
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
