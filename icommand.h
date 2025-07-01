#ifndef ICOMMAND_H
#define ICOMMAND_H

// ICommand: Abstract interface for undoable LCMS operations
class ICommand {
public:
    // Executes the command
    virtual void execute() = 0;

    // Reverts the command's effect (undo)
    virtual void undo() = 0;

    // Virtual destructor
    virtual ~ICommand() = default;
};

#endif // ICOMMAND_H
