package br.lnls.dig.gradle.sedtask

class CommandCursor {
    private ArrayList<Command> commands
    private int commandIndex = 0

    CommandCursor(ArrayList<Command> commands) {
        this.commands = commands
    }

    void next() {
        commandIndex += 1
    }

    void advance(int count) {
        commandIndex += count
    }

    Command getCurrentCommand() {
        return commands.get(commandIndex)
    }

    boolean finished() {
        return commandIndex == commands.size()
    }
}
