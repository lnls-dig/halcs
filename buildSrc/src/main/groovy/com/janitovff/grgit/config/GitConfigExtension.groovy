package com.janitovff.grgit.config

import org.gradle.api.Project

import org.eclipse.jgit.lib.Config

class GitConfigExtension {
    private Config config

    GitConfigExtension(Project project) {
        project.plugins.withId('org.ajoberstar.grgit') {
            this.setConfig(project.grgit.repository.jgit.repository.config)
        }
    }

    protected void setConfig(Config config) {
        this.config = config
    }

    def propertyMissing(String propertyName) {
        String section = propertyName

        return new GitConfigPropertyGetter(config, section)
    }
}
