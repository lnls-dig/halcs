package br.lnls.dig.gradle.assemblevarianttask.plugins

import org.gradle.api.DefaultTask
import org.gradle.api.Task
import org.gradle.model.ModelMap
import org.gradle.model.Mutate
import org.gradle.model.Path
import org.gradle.model.RuleSource
import org.gradle.nativeplatform.BuildTypeContainer
import org.gradle.nativeplatform.FlavorContainer
import org.gradle.nativeplatform.NativeBinarySpec

public class AssembleVariantTaskPlugin extends RuleSource {
    @Mutate
    void addAssembleVariantTasks(ModelMap<Task> tasks,
            BuildTypeContainer buildTypes, FlavorContainer flavors,
            @Path('binaries') ModelMap<NativeBinarySpec> binaries) {
        [buildTypes, flavors].combinations().collect() { combination ->
            def buildType = combination[0]
            def flavor = combination[1]

            def buildTypeName = buildType.name.capitalize()
            def flavorName = flavor.name.capitalize()
            def taskName = "assemble$flavorName$buildTypeName"

            tasks.create(taskName, DefaultTask) { task ->
                def variantBinaries = binaries.findAll() { binary ->
                    binary.buildType == buildType &&
                    binary.flavor == flavor
                }

                variantBinaries.each() { binary ->
                    task.dependsOn binary.buildTask
                }
            }
        }
    }
}
