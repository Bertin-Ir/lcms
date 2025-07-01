#ifndef ADD_BOOK_H
#define ADD_BOOK_H
#include "icommand.h"
#include "lcms.h"

// This class implements the ICommand interface and encapsulates the operation of adding a book
// to the LCMS (Library Catalog Management System). It provides an execute method to perform the
// addition, an undo method to remove the added book, and a method to retrieve the command's name.
class AddBook : public ICommand {
private:
    LCMS* lcms;
    Book* addedBook; // Stores the pointer to the added book

public:
    // Constructor to initialize the command with a LCMS instance and book data
    AddBook(LCMS* lcmsInstance) : lcms(lcmsInstance), addedBook(nullptr) {}

    // Executes the command to add a book and marks as added
    void execute() override {
        addedBook = lcms->addBook(); // Calls the addBook method of LCMS and stores the book name
    }

    // Reverts the command's effect (undo) by removing the added book
    void undo() override {
        if (addedBook) { // Check if a book was added
            Book* dummyBook = nullptr;
            string dummyCategory = "";
            lcms->removeBook(addedBook->getTitle(), dummyBook, dummyCategory); // removes the added book from the LCMS
            delete addedBook; // Deletes the added book to free memory
            addedBook = nullptr; // Reset the pointer to prevent dangling pointer
            
        } 
    }

};

#endif // ADD_BOOK_H