package br.lnls.dig.gradle.nativedistribution.tasks

import org.gradle.api.distribution.Distribution
import org.gradle.api.internal.resolve.ProjectModelResolver
import org.gradle.api.tasks.Exec
import org.gradle.language.nativeplatform.DependentSourceSet
import org.gradle.model.internal.registry.ModelRegistry
import org.gradle.nativeplatform.NativeBinarySpec

public class RpmInstall extends Exec {
    Distribution distribution

    public void setRpmDirectory(File rpmDirectory) {
        commandLine("bash", "-c", "rpm -Uvh --force $rpmDirectory/*")
    }

    public void resolveDependencies(ProjectModelResolver resolver) {
        addDependenciesFrom(distribution.executables, resolver)
        addDependenciesFrom(distribution.sharedLibraries, resolver)
    }

    public void addDependenciesFrom(Set<NativeBinarySpec> binaries,
            ProjectModelResolver resolver) {
        binaries.inputs.each { sources ->
            def sourcesWithDependencies = sources.findAll { source ->
                source instanceof DependentSourceSet
            }

            sourcesWithDependencies.each { source ->
                source.libs.each { lib ->
                    addDetectedDependency(lib, resolver)
                }
            }
        }
    }

    protected void addDetectedDependency(Object dependency,
            ProjectModelResolver resolver) {
        if (dependency instanceof Map)
            addDetectedDependencySpecification((Map)dependency, resolver)
        else
            throw new RuntimeException("Unknown dependency: $dependency")
    }

    private void addDetectedDependencySpecification(Map specification,
            ProjectModelResolver resolver) {
        def projectPath = specification.get("project")
        def libraryName = specification.get("library")
        def projectModel = resolver.resolveProjectModel(projectPath)
        def installTask = getProjectInstallTask(projectPath, projectModel)

        if (installTask != null)
            dependsOn installTask
    }

    private RpmInstall getProjectInstallTask(String projectPath,
            ModelRegistry projectModel) {
        def tasks = projectModel.find("tasks", Object)
        def expectedName = name

        return tasks.find { task -> task.name == expectedName }
    }
}
