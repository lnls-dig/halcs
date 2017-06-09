package br.lnls.dig.gradle.sedtask

import java.util.regex.Pattern

class ReplaceCommand implements Command {
    private Pattern pattern
    private String replacement

    ReplaceCommand(Pattern pattern, String replacement) {
        this.pattern = pattern
        this.replacement = replacement
    }

    void apply(StringBuffer buffer, CommandCursor commandPosition) {
        String line = buffer.toString()
        String updatedLine = pattern.matcher(line).replaceFirst(replacement)

        buffer.delete(0, buffer.length())
        buffer.append(updatedLine)

        commandPosition.next()
    }
}
