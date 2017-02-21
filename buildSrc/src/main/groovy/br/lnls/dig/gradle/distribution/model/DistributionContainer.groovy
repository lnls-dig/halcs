package br.lnls.dig.gradle.distribution.model

import org.gradle.api.distribution.Distribution
import org.gradle.api.ExtensiblePolymorphicDomainObjectContainer

public interface DistributionContainer
        extends ExtensiblePolymorphicDomainObjectContainer<Distribution> {
}
