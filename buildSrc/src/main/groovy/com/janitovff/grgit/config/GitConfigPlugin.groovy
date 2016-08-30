package com.janitovff.grgit.config

import org.gradle.api.Plugin
import org.gradle.api.Project

class GitConfigPlugin implements Plugin<Project> {
    void apply(Project project) {
        project.extensions.create('gitConfig', GitConfigExtension, project)
    }
}
