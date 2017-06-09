package br.lnls.dig.gradle.sedtask

abstract class ConditionalCommand implements Command {
    int commandCount = 0

    public void apply(StringBuffer buffer, CommandCursor commandPosition) {
        if (conditionResult(buffer) != true)
            commandPosition.advance(commandCount)

        commandPosition.next()
    }

    protected abstract boolean conditionResult(StringBuffer buffer)
}
