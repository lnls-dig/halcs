package br.lnls.dig.gradle.distribution.model.internal

import org.gradle.api.distribution.Distribution
import org.gradle.api.internal.DefaultPolymorphicDomainObjectContainer
import org.gradle.api.internal.file.FileOperations
import org.gradle.internal.reflect.Instantiator

import br.lnls.dig.gradle.distribution.model.DistributionContainer

public class DefaultDistributionContainer
        extends DefaultPolymorphicDomainObjectContainer<Distribution>
        implements DistributionContainer {
    private FileOperations fileOperations

    public DefaultDistributionContainer(FileOperations fileOperations,
            Instantiator instantiator) {
        super(Distribution, instantiator)

        this.fileOperations = fileOperations
    }

    public <T extends Distribution> void registerFactory(Class<T> interfaceType,
            Class<? extends T> instanceType) {
        def fileOperations = this.fileOperations

        registerFactory(interfaceType) { name ->
            def constructor = instanceType.getDeclaredConstructor(String,
                    FileOperations)

            return constructor.newInstance(name, fileOperations)
        }
    }
}
