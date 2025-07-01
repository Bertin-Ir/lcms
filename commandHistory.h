// Name         : tree.cpp
// Author       : Bertin Iradukunda
// Version      : 1.0
// Date Created : June 28, 2025
// Date Modified: June 28, 2025
// Description  : This program implements the CommandHistory class, which manages a history of commands.
//============================================================================

#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include "icommand.h"
#include "myvector.h"

// Manages the history of commands for undo functionality
class CommandHistory {
private:
    MyVector<ICommand*> history;

public:
    // Push a command after execution
    void push(ICommand* cmd) {
        history.push_back(cmd);
    }

    // Pop and return the most recent command
    ICommand* pop() {
        if (!isEmpty()) {
            ICommand* last = history.back();
            history.erase(history.size() - 1);
            return last;
        }
        return nullptr;
    }

    // Check if history stack is empty
    // Returns true if there are no commands in history, false otherwise
    bool isEmpty() const {
        return history.empty();
    }

    // Cleanup: delete remaining commands
    ~CommandHistory() {
        for (int i = 0; i < history.size(); ++i) {
            delete history[i];
        }
    }
};

#endif // COMMAND_HISTORY_H
