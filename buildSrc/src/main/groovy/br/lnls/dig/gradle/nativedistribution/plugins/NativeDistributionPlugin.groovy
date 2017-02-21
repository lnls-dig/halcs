package br.lnls.dig.gradle.nativedistribution.plugins

import org.gradle.api.Plugin
import org.gradle.api.Project

import br.lnls.dig.gradle.distribution.plugins.DistributionPlugin
import br.lnls.dig.gradle.nativedistribution.plugins.CurrentPlatformPlugin
import br.lnls.dig.gradle.nativedistribution.plugins.DefaultNativeDistributionsPlugin
import br.lnls.dig.gradle.nativedistribution.plugins.RpmDistributionPlugin

class NativeDistributionPlugin implements Plugin<Project> {
    void apply(Project project) {
        project.pluginManager.apply(DistributionPlugin)
        project.pluginManager.apply(CurrentPlatformPlugin)
        project.pluginManager.apply(DefaultNativeDistributionsPlugin)
        project.pluginManager.apply(RpmDistributionPlugin)
        project.pluginManager.apply(RpmInstallationPlugin)
    }
}
