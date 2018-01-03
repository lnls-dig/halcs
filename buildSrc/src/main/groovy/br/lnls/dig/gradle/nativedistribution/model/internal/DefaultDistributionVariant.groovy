package br.lnls.dig.gradle.nativedistribution.model.internal

import org.gradle.api.distribution.Distribution
import org.gradle.api.distribution.internal.DefaultDistribution
import org.gradle.api.file.FileCollection
import org.gradle.api.internal.file.FileOperations
import org.gradle.api.internal.project.ProjectIdentifier
import org.gradle.api.internal.resolve.ProjectModelResolver
import org.gradle.api.Task
import org.gradle.language.nativeplatform.DependentSourceSet
import org.gradle.language.nativeplatform.HeaderExportingSourceSet
import org.gradle.model.internal.registry.ModelRegistry
import org.gradle.nativeplatform.BuildType
import org.gradle.nativeplatform.Flavor
import org.gradle.nativeplatform.NativeBinarySpec
import org.gradle.nativeplatform.NativeExecutableBinarySpec
import org.gradle.nativeplatform.NativeLibrarySpec
import org.gradle.nativeplatform.SharedLibraryBinarySpec
import org.gradle.platform.base.ComponentSpecContainer
import org.gradle.platform.base.Platform

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.nativedistribution.model.DistributionVariant
import br.lnls.dig.gradle.nativedistribution.model.internal.Dependency
import br.lnls.dig.gradle.sysfiles.model.SysFilesSet

import static org.gradle.api.distribution.plugins.DistributionPlugin.MAIN_DISTRIBUTION_NAME

public class DefaultDistributionVariant extends DefaultDistribution
        implements DistributionVariant {
    ProjectIdentifier project

    BuildType buildType
    Flavor flavor
    Platform platform
    String usage

    Set<NativeExecutableBinarySpec> executables
    Set<SharedLibraryBinarySpec> sharedLibraries
    Set<NativeBinarySpec> binariesWithSysFiles

    Set<Dependency> dependencies

    private FileOperations fileOperations

    DefaultDistributionVariant(String name, FileOperations fileOperations) {
        super(name, fileOperations.copySpec())

        executables = new LinkedHashSet<>()
        sharedLibraries = new LinkedHashSet<>()
        binariesWithSysFiles = new LinkedHashSet<>()
        dependencies = new LinkedHashSet<>()

        this.fileOperations = fileOperations
    }

    String taskNameFor(String suffix) {
        if (name == MAIN_DISTRIBUTION_NAME)
            return suffix
        else
            return name + suffix.capitalize()
    }

    void addBinaries(Iterable<NativeBinarySpec> binaries) {
        def compatibleBinaries = binaries.findAll() { binary ->
            binary.buildType.name == buildType.name &&
            binary.flavor.name == flavor.name &&
            binary.targetPlatform.name == platform.name
        }

        compatibleBinaries.each { binary ->
            if (binary instanceof NativeExecutableBinarySpec)
                executables.add((NativeExecutableBinarySpec)binary)
            if (binary instanceof SharedLibraryBinarySpec)
                sharedLibraries.add((SharedLibraryBinarySpec)binary)
            if (hasSysFilesSet(binary))
                binariesWithSysFiles.add(binary)
        }
    }

    FileCollection getExecutableFiles() {
        def executableFiles = executables.collect { it.executable.file }

        return fileOperations.files(executableFiles)
    }

    FileCollection getSharedLibraryFiles() {
        def libraryFiles = sharedLibraries.collect { it.sharedLibraryFile }

        return fileOperations.files(libraryFiles)
    }

    FileCollection getExportedHeaders() {
        def sourceSets = sharedLibraries.collect { library ->
            library.inputs.withType(HeaderExportingSourceSet)
        }

        def headers = sourceSets.sum().collect { sourceSet ->
            sourceSet.exportedHeaders
        }

        return headers.sum() ?: fileOperations.files()
    }

    FileCollection getSysFiles() {
        def sysFilesSets = binariesWithSysFiles.collect { binary ->
            def sourceSets = binary.inputs.findAll { sourceSet ->
                sourceSet instanceof SysFilesSet
            }

            sourceSets.collect { it.source }.sum()
        }

        return sysFilesSets.sum() ?: fileOperations.files()
    }

    boolean hasSysFilesSet(NativeBinarySpec binary) {
        def sysFilesSets = binary.inputs.findAll { it instanceof SysFilesSet }

        return !sysFilesSets.isEmpty()
    }

    @Override
    public Set<Task> getBuildTasks() {
        def buildTasks = new HashSet<Task>()

        buildTasks.addAll(executables.collect { it.buildTask })
        buildTasks.addAll(sharedLibraries.collect { it.buildTask })

        return buildTasks
    }

    void resolveDependencies(ProjectModelResolver resolver) {
        resolveDependenciesOf(executables, resolver)
        resolveDependenciesOf(sharedLibraries, resolver)
    }

    private void resolveDependenciesOf(Set<NativeBinarySpec> binaries,
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
        def projectPath =  specification.get("project")

        if (projectPath != null && projectPath != project.path) {
            addDetectedExternalDependencySpecification(projectPath,
                specification, resolver)
        }
    }

    private void addDetectedExternalDependencySpecification(String projectPath,
            Map specification, ProjectModelResolver resolver) {
        def libraryName = specification.get("library")
        def linkageType = specification.get("linkage")
        def projectModel = resolver.resolveProjectModel(projectPath)
        def distribution = getProjectDistribution(projectPath, projectModel)
        def project = projectModel.find("projectIdentifier", Object)
        def version = project?.version.toString()

        if (distribution != null) {
            dependencies.add(new Dependency(projectPath, version, distribution))
        } else if (linkageType != "api")
            includeFilesFromProjectLibraryDependency(libraryName, projectModel)
    }

    Distribution getProjectDistribution(String projectPath,
            ModelRegistry projectModel) {
        DistributionContainer distributions = projectModel.find("distributions",
                DistributionContainer)

        Distribution distribution = distributions?.find { distribution ->
            distribution.name == name
        }

        return distribution
    }

    private void includeFilesFromProjectLibraryDependency(String libraryName,
            ModelRegistry projectModel) {
        def library = getDependencyLibrary(libraryName, projectModel)

        addBinaries(library.binaries)
    }

    private NativeLibrarySpec getDependencyLibrary(String libraryName,
            ModelRegistry projectModel) {
        def components = projectModel.find("components", ComponentSpecContainer)

        return components.withType(NativeLibrarySpec).get(libraryName)
    }
}
