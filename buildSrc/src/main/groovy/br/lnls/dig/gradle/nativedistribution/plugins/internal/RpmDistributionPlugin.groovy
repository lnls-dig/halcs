package br.lnls.dig.gradle.nativedistribution.plugins

import org.gradle.api.Task
import org.gradle.model.Each
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.tasks.Rpm

class RpmDistributionPlugin extends RuleSource {
    @Mutate
    public void addNativeDistributionTasks(ModelMap<Task> tasks,
            DistributionContainer distributions,
            @Path("buildDir") File buildDir) {
        distributions.all { distribution ->
            tasks.create(distribution.taskNameFor("distRpm"), Rpm) {
                    task ->
                task.distribution = distribution
                task.dependsOn(distribution.buildTasks)
                task.buildDir = buildDir
                task.outputDirectory = new File(buildDir,
                        "/distributions/$distribution.name")
            }
        }
    }
}
