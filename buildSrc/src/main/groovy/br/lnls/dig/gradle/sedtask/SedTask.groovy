package br.lnls.dig.gradle.sedtask

import java.util.regex.Pattern

import org.gradle.api.DefaultTask
import org.gradle.api.tasks.TaskAction

public class SedTask extends DefaultTask {
    private ArrayList<Command> commandList = new ArrayList<Command>()
    private SedCommands commands = new SedCommands(commandList)

    File file

    void file(File file) {
        this.file = file
    }

    void when(Pattern pattern, Closure commandBlock) {
        commands.when(pattern, commandBlock)
    }

    void replace(Pattern pattern, String replacement) {
        commands.replace(pattern, replacement)
    }

    @TaskAction
    void run() {
        if (file == null) {
            def message = "File to edit must be configured for SedTask $name"
            throw new IllegalArgumentException(message)
        }

        List<String> updatedLines = new ArrayList<String>()

        file.eachLine { line ->
            String updatedLine = runCommands(line)

            updatedLines.add(updatedLine)
        }

        file.withPrintWriter { writer ->
            updatedLines.each { line -> writer.println(line) }
        }
    }

    String runCommands(String line) {
        StringBuffer buffer = new StringBuffer(line)
        CommandCursor cursor = new CommandCursor(commandList)

        while (!cursor.finished()) {
            Command command = cursor.currentCommand

            command.apply(buffer, cursor)
        }

        return buffer.toString()
    }
}
