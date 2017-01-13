package br.lnls.dig.gradle.nativerelease.plugins

import org.gradle.api.distribution.Distribution
import org.gradle.api.Task
import org.gradle.model.Each
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.tasks.Rpm
import br.lnls.dig.gradle.nativerelease.tasks.CollectRpmsTask

import static org.gradle.api.distribution.plugins.DistributionPlugin.MAIN_DISTRIBUTION_NAME

public class NativeReleasePlugin extends RuleSource {
    @Mutate
    void addCollectRpmsTasks(ModelMap<Task> tasks,
            DistributionContainer distributions,
            @Path("buildDir") File buildDir) {
        distributions.all { distribution ->
                def buildTypeName = buildType.name.capitalize()
            def taskName = "${distribution.name}CollectRpms"
            def rpmTaskName = getTaskNameFor(distribution, "distRpm")

            tasks.create(taskName, CollectRpmsTask) { task ->
                task.into "$task.project.rootProject.buildDir/release"
                task.from "$buildDir/distributions/$distribution.name"

                task.rename '(.*)', "$distribution.name-\$1"

                task.dependsOn rpmTaskName
            }
        }
    }

    private String getTaskNameFor(Distribution distribution,
            String taskSuffix) {
        if (distribution.name == MAIN_DISTRIBUTION_NAME)
            return taskSuffix
        else
            return distribution.name + taskSuffix.capitalize()
    }
}
