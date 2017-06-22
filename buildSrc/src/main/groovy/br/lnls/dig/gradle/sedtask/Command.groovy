package br.lnls.dig.gradle.sedtask

interface Command {
    void apply(StringBuffer buffer, CommandCursor commandPosition)
}
