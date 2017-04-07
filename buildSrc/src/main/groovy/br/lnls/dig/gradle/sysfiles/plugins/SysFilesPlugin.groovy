package br.lnls.dig.gradle.sysfiles.plugins

import org.gradle.language.base.LanguageSourceSet
import org.gradle.nativeplatform.NativeBinarySpec

import org.gradle.model.Each
import org.gradle.model.Mutate
import org.gradle.model.RuleSource
import org.gradle.model.Validate
import org.gradle.nativeplatform.NativeComponentSpec
import org.gradle.platform.base.ComponentType
import org.gradle.platform.base.TypeBuilder

import br.lnls.dig.gradle.sysfiles.model.internal.DefaultSysFilesSet
import br.lnls.dig.gradle.sysfiles.model.SysFilesSet

public class SysFilesPlugin extends RuleSource {
    @ComponentType
    void registerSysFilesSet(TypeBuilder<SysFilesSet> builder) {
        builder.defaultImplementation(DefaultSysFilesSet)
    }

    @Mutate
    void createSysFilesSourceSets(@Each NativeComponentSpec component) {
        component.sources.create('sysfiles', SysFilesSet)
    }
}
