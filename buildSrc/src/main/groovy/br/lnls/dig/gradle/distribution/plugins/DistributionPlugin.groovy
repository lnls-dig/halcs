package br.lnls.dig.gradle.distribution.plugins

import javax.inject.Inject

import org.gradle.api.distribution.Distribution
import org.gradle.api.distribution.internal.DefaultDistribution
import org.gradle.api.internal.file.FileOperations
import org.gradle.api.Plugin
import org.gradle.api.plugins.ExtensionContainer
import org.gradle.api.Project
import org.gradle.internal.reflect.Instantiator
import org.gradle.model.Finalize
import org.gradle.model.Model
import org.gradle.model.Mutate
import org.gradle.model.RuleSource

import br.lnls.dig.gradle.distribution.model.DistributionContainer
import br.lnls.dig.gradle.distribution.model.internal.DefaultDistributionContainer

import static org.gradle.api.distribution.plugins.DistributionPlugin.MAIN_DISTRIBUTION_NAME

class DistributionPlugin implements Plugin<Project> {
    private FileOperations fileOperations
    private Instantiator instantiator

    @Inject
    public DistributionPlugin(FileOperations fileOperations,
            Instantiator instantiator) {
        this.fileOperations = fileOperations
        this.instantiator = instantiator
    }

    @Override
    void apply(Project project) {
        project.extensions.create("distributions", DefaultDistributionContainer,
                fileOperations, instantiator)
    }

    public static class Rules extends RuleSource {
        @Model
        public DistributionContainer distributions(
                ExtensionContainer extensions) {
            return extensions.getByType(DistributionContainer)
        }

        @Mutate
        public void addDefaultDistributionFactory(
                DistributionContainer distributions) {
            distributions.registerFactory(Distribution, DefaultDistribution)
        }

        @Finalize
        public void addMainDistributionIfNoneWereDefined(
                DistributionContainer distributions) {
            if (distributions.isEmpty())
                distributions.create("main")
        }
    }
}
