package com.janitovff.grgit.config

import org.eclipse.jgit.lib.Config

class GitConfigPropertyGetter {
    private Config config

    private String section
    private String subSection

    GitConfigPropertyGetter(Config config, String section) {
        this.config = config
        this.section = section
        this.subSection = null
    }

    GitConfigPropertyGetter(Config config, String section, String subSection) {
        this.config = config
        this.section = section
        this.subSection = subSection
    }

    String getProperty(String name) {
        Set<String> subSections = getSubSections()

        if (subSection == null && subSections.contains(name))
            return new GitConfigPropertyGetter(config, section, name)
        else
            return config.getString(section, subSection, name)
    }

    Set<String> getSubSections() {
        try {
            return config.getSubsections()
        } catch (NullPointerException exception) {
            return new HashSet<String>()
        }
    }
}
