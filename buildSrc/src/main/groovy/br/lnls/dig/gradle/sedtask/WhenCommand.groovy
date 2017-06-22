package br.lnls.dig.gradle.sedtask

import java.util.regex.Pattern

class WhenCommand extends ConditionalCommand {
    private Pattern pattern

    public WhenCommand(Pattern pattern) {
        this.pattern = pattern
    }

    protected boolean conditionResult(StringBuffer buffer) {
        return pattern.matcher(buffer.toString()).find()
    }
}
