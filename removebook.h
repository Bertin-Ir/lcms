// Name         : tree.cpp
// Author       : Bertin Iradukunda
// Version      : 1.0
// Date Created : June 28, 2025
// Date Modified: June 29, 2025
// Description  : This file implements the ICommand interface and provides concrete command classes for adding, removing, and editing books in the LCMS.
//============================================================================
#ifndef REMOVE_BOOK_H
#define REMOVE_BOOK_H

#include"icommand.h"
#include"lcms.h"
// This class implements the ICommand interface and encapsulates the operation of removing a book
// from the LCMS (Library Catalog Management System). It provides an execute method to perform the
// removal, an undo method to restore the removed book, and a method to retrieve the command's name.
class RemoveBook : public ICommand {
private:
    LCMS* lcms;
    std::string bookTitle;
    Book* removedBook; // Stores the removed book's data
    string removedBookCategory; // Stores the category of the removed book
    bool undone; // Flag to track if the command has been undone

public:
    // Constructor to initialize the command with a LCMS instance and book title
    RemoveBook(LCMS* lcmsInstance, const std::string& title) : lcms(lcmsInstance), bookTitle(title) {
        removedBook = nullptr; // Initialize removedBook to nullptr
        removedBookCategory = ""; // Initialize removedBookCategory to an empty string
        undone = false; // Initialize undone to false
    }

    // Executes the command to remove a book and stores its data
    void execute() override {
        lcms->removeBook(bookTitle, removedBook, removedBookCategory); // Calls the removeBook method of LCMS
    }

    // Reverts the command's effect (undo) by re-adding the removed book
    void undo() override {
        if (removedBook != nullptr) {
            lcms->addBook(removedBook, removedBookCategory); // Re-adds the removed book to the LCMS
            undone = true; // Marks the command as undone
            removedBook = nullptr; // Resets removedBook to nullptr after undo to prevent double undo
        }   
        
    }

    ~RemoveBook() {
        // If the book was removed and never re-added, delete it
        if (removedBook && !undone) {
            delete removedBook;
        }
    }
    

};

#endif // REMOVE_BOOK_H

// End of file

