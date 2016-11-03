package br.lnls.dig.gradle.sanec.plugins

import org.gradle.language.c.CSourceSet
import org.gradle.model.Each
import org.gradle.model.Mutate
import org.gradle.model.RuleSource

public class SaneCPlugin extends RuleSource {
    @Mutate
    void restrictSourceSetFiles(@Each CSourceSet sourceSet) {
        if (sourceSet.source.includes.isEmpty()) {
            sourceSet.source.include '**/*.c'
            sourceSet.source.include '**/*.h'
        }
    }
}
