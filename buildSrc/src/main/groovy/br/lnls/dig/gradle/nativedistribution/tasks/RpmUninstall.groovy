package br.lnls.dig.gradle.nativedistribution.tasks

//import javax.inject.Inject

import org.gradle.api.distribution.Distribution
import org.gradle.api.tasks.Exec
//import org.gradle.model.internal.registry.ModelRegistry

import br.lnls.dig.gradle.nativedistribution.model.internal.RpmDistribution

public class RpmUninstall extends Exec {
    private RpmDistribution distribution

    //@Inject
    //public ModelRegistry getProjectModel() {
        //String message = 'Project\'s ModelRegistry not injected'
        //throw new UnsupportedOperationException(message)
    //}

    public void setDistribution(Distribution distribution) {
        this.distribution = new RpmDistribution(distribution)

        commandLine 'rpm', '-e', packageName

        //processDependencies()
    }

    String getPackageName() {
        if (distribution.isDevelopment())
            return project.name + '-devel'
        else
            return project.name
    }

    //private void processDependencies() {
        //System.err.println("$project.name - $distribution.name {")
        //distribution.dependencies.each { dependency ->
            //def uninstallTask = dependency.uninstallTask

            //System.err.println("    Dependency: $dependency.name")
            //System.err.println("        Uninstall: $uninstallTask")

            //if (uninstallTask != null)
                //uninstallTask.dependsOn this
        //}
        //System.err.println("$project.name - $distribution.name }")
    //}
}
