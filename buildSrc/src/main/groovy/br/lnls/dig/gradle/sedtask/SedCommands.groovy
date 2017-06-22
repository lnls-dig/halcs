package br.lnls.dig.gradle.sedtask

import java.util.regex.Pattern

class SedCommands {
    private List<Command> commands

    SedCommands(List<Command> commands) {
        this.commands = commands
    }

    void when(Pattern pattern, Closure commandBlock) {
        WhenCommand command = new WhenCommand(pattern)
        int commandBlockStart = commands.size() + 1

        commands.add(command)

        commandBlock.resolveStrategy = Closure.DELEGATE_FIRST
        commandBlock.delegate = this
        commandBlock()

        int commandBlockEnd = commands.size()
        int commandBlockSize = commandBlockEnd - commandBlockStart

        command.commandCount = commandBlockSize
    }

    void replace(Pattern pattern, String replacement) {
        commands.add(new ReplaceCommand(pattern, replacement))
    }
}
