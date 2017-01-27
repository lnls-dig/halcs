package br.lnls.dig.gradle.nativedistribution.plugins

import org.gradle.api.Task
import org.gradle.model.Each
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.tasks.RpmInstall

class RpmInstallationPlugin extends RuleSource {
    @Mutate
    public void addInstallationTasks(ModelMap<Task> tasks,
            DistributionContainer distributions,
            @Path("buildDir") File buildDir) {
        distributions.all { distribution ->
            tasks.create(distribution.taskNameFor("installRpm"),
                    RpmInstall) { task ->
                task.distribution = distribution
                task.rpmDirectory = new File(buildDir,
                        "/distributions/$distribution.name")
            }
        }
    }
}
