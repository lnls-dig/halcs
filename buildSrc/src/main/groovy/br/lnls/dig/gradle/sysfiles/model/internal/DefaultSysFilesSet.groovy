package br.lnls.dig.gradle.sysfiles.model.internal

import org.gradle.language.base.sources.BaseLanguageSourceSet

import br.lnls.dig.gradle.sysfiles.model.SysFilesSet

public class DefaultSysFilesSet extends BaseLanguageSourceSet
        implements SysFilesSet {
    @Override
    public String getLanguageName() {
        return "System files"
    }
}
