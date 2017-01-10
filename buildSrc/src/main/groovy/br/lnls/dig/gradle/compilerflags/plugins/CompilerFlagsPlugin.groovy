package br.lnls.dig.gradle.compilerflags.plugins

import org.gradle.model.Each
import org.gradle.model.Mutate
import org.gradle.model.RuleSource
import org.gradle.nativeplatform.NativeBinarySpec

public class CompilerFlagsPlugin extends RuleSource {
    @Mutate
    void applyCompilerFlags(@Each NativeBinarySpec binary) {
        binary.cCompiler.args getCFlags()
        binary.cppCompiler.args getCppFlags()
        binary.linker.args getLdFlags()
    }

    private String[] getCFlags() {
        return getFlags("CFLAGS")
    }

    private String[] getCppFlags() {
        return getFlags("CPPFLAGS") + getFlags("CXXFLAGS")
    }

    private String[]  getLdFlags() {
        return getFlags("LDFLAGS")
    }

    private String[] getFlags(String variable) {
        def value = System.getenv(variable)

        if (value == null)
            return new String[0]
        else
            return value.split(" ")
    }
}
