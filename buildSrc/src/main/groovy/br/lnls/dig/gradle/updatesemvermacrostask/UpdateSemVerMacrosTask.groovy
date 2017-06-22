package br.lnls.dig.gradle.updatesemvermacrostask

import br.lnls.dig.gradle.sedtask.SedTask

public class UpdateSemVerMacrosTask extends SedTask {
    void macroPrefix(String macroPrefix) {
        def semVerNums = semVerNumbers
        def major = semVerNums[0]
        def minor = semVerNums[1]
        def patch = semVerNums[2]

        when(~"#define ${macroPrefix}MAJOR [0-9]*") {
            replace ~/[0-9]*$/, major
        }

        when(~"#define ${macroPrefix}MINOR [0-9]*") {
            replace ~/[0-9]*$/, minor
        }

        when(~"#define ${macroPrefix}PATCH [0-9]*") {
            replace ~/[0-9]*$/, patch
        }
    }

    String getSemVerSummary() {
        return project.version.toString().split('-')[0]
    }

    String[] getSemVerNumbers() {
        return semVerSummary.split('[.]')
    }
}
