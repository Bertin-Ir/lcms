// Name         : lcms.cpp
// Author       : Bertin Iradukunda
// Version      : 1.0
// Date Created : April 12, 2025
// Date Modified: June 30, 2025
// Description  : The LCMS class represents a Library Content Management System that uses a tree structure 
//to store books and categories
//============================================================================

#include "lcms.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
// Define color codes
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RESET   "\033[0m"

// Constructor to initialize the LCMS with a root category name 
LCMS::LCMS(string name) {
    libTree = new Tree(name); // Create a Tree with the given name
}

// Destructor to clean allocated memory
LCMS::~LCMS() {
    delete libTree; // Delete the Tree to free up memory
}

// This method import data from a file located at the given path
int LCMS::import(string path) {
    ifstream infile(path); // Open the file in reading mode at the given path
    if (!infile.is_open()) { // Check if the infile is open
        cerr << COLOR_RED << "Could not open file " << path << COLOR_RESET << endl; // Print an error message if the file opening fails
        return -1; 
    }

    string line;
    int count = 0; // Initialize a counter for the number of books imported
    getline(infile, line); // Read the header line from the file for purpose of skipping it
    while (getline(infile, line)) { // Read each book from the file
        MyVector<string> book_details;
        string detail;
        bool inquotes = false; // Flag to track if we are inside quotes
        // check if the line is empty
        if (line.empty()) {
            continue; // Skip empty strings
        }

        // Parse the line into book details
        for (char ch : line) {
            if (ch == '"') {
                inquotes = !inquotes; // update inquotes state
            } 
            else if (ch == ',' && !inquotes) {
                book_details.push_back(detail);
                detail.clear();
            } 
            else {
                detail += ch; // Append character to detail
            }
        }
        book_details.push_back(detail); // Add the last detail

        // Check if we have enough book_details
        if (book_details.size() < 5) {
            cerr << COLOR_RED << "Incomplete book details" << COLOR_RESET << endl;
            continue;
        }

        string title = book_details[0];
        string author = book_details[1];
        string isbn = book_details[2];
        string year = book_details[3];
        string category = book_details[4];
        Node* categoryNode = libTree->getNode(category); // Get the category node from the tree
        if (categoryNode == nullptr) { // If the category node does not exist
            categoryNode = libTree->createNode(category); // Create a new category node in the tree
        }
        // Check for duplicate book in the category
        bool duplicate = false;
        for (int i = 0; i < categoryNode->books.size(); i++) {
            Book* book = categoryNode->books[i];
            if (book->title == title && book->author == author && book->isbn == isbn) {
                cout << COLOR_RED << "The book \"" << title << "\" by " << author << " already exists in the catalog." << COLOR_RESET << endl;
                duplicate = true;
                break; // Skip adding the book if already exists
            }
        }
        if (duplicate) continue;
        Book* newBook = nullptr; 
        try {
            int publicationYear = stoi(year); // Convert year to integer
            newBook = new Book(title, author, isbn, publicationYear); // Create a new Book object with the parsed data
        } 
        catch (const invalid_argument& e) {
            continue;
        } 
        
        categoryNode->books.push_back(newBook); // Add the new book to the category node's books vector
        libTree->updateBookCount(categoryNode, 1); // Update the book count in the category node and its parent nodes
        count++; // Increment the counter for each book imported
    }

    infile.close(); // Close the infile

    cout << COLOR_GREEN << count << " books imported successfully." << COLOR_RESET << endl; // Print the number of books imported

    return 0; // Return 0 to indicate success
}

// This method export data from the library to a file located at the given path
void LCMS::exportData(string path) {
    ofstream outfile(path); // Open the file for writing at the given path
    if (!outfile.is_open()) { // Check if the outfile is open
        cerr << COLOR_RED << "Could not open file " << path << COLOR_RESET << endl; // Print an error message if the file opening fails
        return; 
    }

    outfile << "Title,Author,ISBN,Year,Category" << endl; // Write the header line to the file

    int count = libTree->exportData(libTree->getRoot(), outfile); // Export data from the root node of the tree

    outfile.close(); // Close the outfile

    cout << COLOR_GREEN << count << " books exported successfully to " << path << COLOR_RESET << endl;
}

// Method to find all books and categories containing the keyword
// It takes a keyword as input and searches for it in the library
void LCMS::find(string keyword) {
    MyVector<Node*> categoriesFound; // Vector to store found categories
    MyVector<Book*> booksFound; // Vector to store found books

    libTree->find(libTree->getRoot(), keyword, categoriesFound, booksFound); // Call the find method on the root node
    
    cout << categoriesFound.size() << " categories found." << endl; // Print the number of categories found
    cout << booksFound.size() << " books found." << endl; // Print the number of books found
    cout << "----------------------------------------" << endl; // Print a separator line 

    if (categoriesFound.size() > 0) {
        cout << "List of categories containing <" << keyword << ">:" << endl;
        for (int i = 0; i < categoriesFound.size(); i++) {
            cout << i+1 << ". " << categoriesFound[i]->name << endl; // Print the names of found categories
        }
        cout << "========================================" << endl; // Print a separator line 
    }

    // Print the details of found books
    if (booksFound.size() > 0) {
        cout << "List of books containing <" << keyword << ">:" << endl;
        for (int i = 0; i < booksFound.size(); i++) {
            booksFound[i]->print(); // Print the details of found books
        }
    }
}

// Method to find and display all books under a specific category/subcategory
// If no category is provided, it lists all books in the library
void LCMS::findAll(string category) {
    if (category == "") { // If no category is provided
        libTree->printAll(libTree->getRoot()); // Print all books from the root node (entire library)
        cout << libTree->getRoot()->bookCount << " books found" << endl;
        return; 
    }
    Node* categoryNode = libTree->getNode(category); // Get the category node from the tree
    if (categoryNode == nullptr) { // If the category node does not exist
        cout << COLOR_RED << "Category \"" << category << "\" not found in the library catalog" << COLOR_RESET << endl; // Print an error message
        return; 
    }
    libTree->printAll(categoryNode); // Call the printAll method on the category node
    cout << categoryNode->bookCount << " books found" << endl; // Print the number of books found
}

// Method to list all categories and books in the library
void LCMS::list() {
    libTree->print(); // Call the print method on the tree to display all categories and books
}

// Method to find and display details of a book by its title
// It takes the book title as input and searches for it in the library
void LCMS::findBook(string bookTitle) {
    if (bookTitle.empty()) { // If no book title is provided
        cout << COLOR_RED << "No title provided. Please enter a book title." << COLOR_RESET << endl; // Print an error message
        return; 
    }
    Node* StartNode= libTree->getRoot(); // Start from the root node
    Book* foundBook = libTree->findBook(StartNode, bookTitle); // Call the findBook method on the root node

    if (foundBook == nullptr) { // If the book is not found
        cout << COLOR_RED << bookTitle << " not found." << COLOR_RESET << endl; // Print an error message
    }
}

// Method to add a new book to the library
Book* LCMS::addBook() {
    string title, author, isbn, year, category; // Variables to store book details

    // Prompt and read book details [title, author, isbn, publication_year, and category] from user
    do {
        cout << "Enter book title: ";
        getline(cin, title);
        if (title.empty()) {
            cout << COLOR_RED << "Title cannot be empty. Please enter a title." << COLOR_RESET << endl;
        }
    } while (title.empty()); // Read title until a non-empty string is provided

    do {
        cout << "Enter book author: ";
        getline(cin, author);
        if (author.empty()) {
            cout << COLOR_RED << "Author cannot be empty. Please enter author name." << COLOR_RESET << endl;
        }
    } while (author.empty()); // Read author name until a non-empty string is provided

    do {
        cout << "Enter book ISBN: ";
        getline(cin, isbn);
        if (isbn.empty()) {
            cout << COLOR_RED << "ISBN cannot be empty. Please enter a valid ISBN." << COLOR_RESET << endl;
        }
    } while (isbn.empty()); // Read ISBN until a non-empty string is provided

    do {
        cout << "Enter publication year: ";
        getline(cin, year);
        try {
            stoi(year); // Validate if the year is a valid integer
            break;
        } catch (const invalid_argument&) {
            cout << COLOR_RED << "Invalid year format. Please enter a valid year (integer)." << COLOR_RESET << endl;
        }
    } while (true); // Read publication year until a valid year is provided

    do {
        cout << "Enter category: ";
        getline(cin, category);
        if (category.empty()) {
            cout << COLOR_RED << "Category cannot be empty. Please enter a valid category." << COLOR_RESET << endl;
        }
    } while (category.empty()); // Read category until a non-empty string is provided

    Node* categoryNode = libTree->getNode(category); // Get the category node from the tree

    if (categoryNode == nullptr) { // If the category node does not exist
        libTree->createNode(category); // Create a new category node in the tree
        categoryNode = libTree->getNode(category); // Get the newly created category node
    }

    // Check for duplicate book in the category
    for (int i = 0; i < categoryNode->books.size(); i++) {
        Book* book = categoryNode->books[i];
        if (book->title == title && book->author == author && book->isbn == isbn) {
            cout << COLOR_RED << "The book \"" << title << "\" by " << author << " already exists in the catalog." << COLOR_RESET << endl;
            return nullptr; // Skip adding the book if already exists
        }
    }
    int publicationYear = stoi(year); // Convert year to integer
    Book* newBook = new Book(title, author, isbn, publicationYear); // Create a new Book
    categoryNode->books.push_back(newBook); // Add the new book to the category node's books vector

    libTree->updateBookCount(categoryNode, 1); // Update the book count in the category node and its parent nodes
    cout << COLOR_GREEN << title << " has been successfully added into the catalog." << COLOR_RESET << endl; // Print a success message
    return newBook ; // Return the title of the added book
}

// This method is non interactive method for adding a new book to the library
// addBook method is overloaded to allow adding a book without user interaction
void LCMS:: addBook(Book* book, const string category) {
    if (book == nullptr) { // If the book pointer is null
        cout << COLOR_RED << "No book provided. Please provide a valid book." << COLOR_RESET << endl; // Print an error message
        return; 
    }
    Node* categoryNode = libTree->getNode(category); // Get the category node from the tree

    if (categoryNode == nullptr) { // If the category node does not exist
        libTree->createNode(category); // Create a new category node in the tree
        categoryNode = libTree->getNode(category); // Get the newly created category node
    }

    // Check for duplicate book in the category
    for (int i = 0; i < categoryNode->books.size(); i++) {
        Book* existingBook = categoryNode->books[i];
        if (existingBook->title == book->title && existingBook->author == book->author && existingBook->isbn == book->isbn) {
            cout << COLOR_RED << "The book \"" << book->title << "\" by " << book->author << " already exists in the catalog." << COLOR_RESET << endl;
            return; // Skip adding the book if already exists
        }
    }
    
    categoryNode->books.push_back(book); // Add the new book to the category node's books vector
    libTree->updateBookCount(categoryNode, 1); // Update the book count in the category node and its parent nodes
}

// Method to edit the details of an existing book by its title
void LCMS::editBook(string bookTitle) {
    if (bookTitle == "") { // If no book title is provided
        cout << COLOR_RED << "No title provided. Please enter a book title." << COLOR_RESET << endl; // Print an error message
        return; 
    }
    Node* StartNode= libTree->getRoot(); // Start from the root node
    Book* foundBook = libTree->findBook(StartNode, bookTitle); // Call the findBook method on the root node

    if (foundBook == nullptr) { // If the book is not found
        cout << COLOR_RED << bookTitle << " not found." << COLOR_RESET << endl; // Print an error message
        return; 
    }

    string newTitle, newAuthor, newISBN, newYear; // Variables to store new book details
    bool quit = true; // Flag to track if the user wants to quit
    do {
        // display the options for editing
        cout << "1. Title" << endl; 
        cout << "2. Author" << endl; 
        cout << "3. ISBN" << endl; 
        cout << "4. Publication Year" << endl; 
        cout << "5. Exit" << endl; 
        int option;
        cout << "Select the field you want to edit (1-5): ";
        cin >> option; // Read user choice
        if (cin.fail()) { // Check if the input type is valid
            cin.clear(); 
            cin.ignore(); 
            cout << COLOR_RED << "Invalid input. Please enter a number between 1 and 5." << COLOR_RESET << endl; // Print an error message
            continue; 
        }
        cin.ignore(); // Ignore leftover '\n'
        switch (option) {
            case 1:
                do {
                    cout << "Enter new title: ";
                    getline(cin, newTitle); // Read the new title from user input
                    if (newTitle.empty()) {
                        cout << COLOR_RED << "Title cannot be empty. Please enter a valid title." << COLOR_RESET << endl;
                    }
                } while (newTitle.empty()); // Prompt until a valid title is provided
                foundBook->title = newTitle; // Update the book title
                break;
            case 2:
                do {
                    cout << "Enter new author: ";
                    getline(cin, newAuthor); 
                    if (newAuthor.empty()) {
                        cout << COLOR_RED << "Author cannot be empty. Please enter a valid author name." << COLOR_RESET << endl;
                    }
                } while (newAuthor.empty()); // Prompt until a valid author name is provided
                foundBook->author = newAuthor; // Update the book author
                break;
            case 3:
                do {
                    cout << "Enter new ISBN: ";
                    getline(cin, newISBN); 
                    if (newISBN.empty()) {
                        cout << COLOR_RED << "ISBN cannot be empty. Please enter a valid ISBN." << COLOR_RESET << endl;
                    }
                } while (newISBN.empty()); // Prompt until a valid ISBN is provided
                foundBook->isbn = newISBN; // Update the book ISBN
                break;
            case 4:
                do {
                    cout << "Enter new publication year: ";
                    getline(cin, newYear); // Read the new publication year from user input
                    if (newYear.empty()) {
                    cout << COLOR_RED << "Publication year cannot be empty. Please enter a valid year." << COLOR_RESET << endl;
                    continue;
                    }
                    try {
                    foundBook->publication_year = stoi(newYear); // Update the book publication year if valid
                    break;
                    } 
                    catch (const invalid_argument&) {
                    cout << COLOR_RED << "Invalid year format. Please enter a valid year (integer)." << COLOR_RESET << endl;
                    }
                } while (true); // Prompt until a valid year is provided
                break;
        case 5:
            quit = false; // Set quit to true to exit the loop
            break;
        default:
            cout << COLOR_RED << "Invalid option. Please select 1-5." << COLOR_RESET << endl; 
    }
    } while (quit); // Keep prompting until the user decides to quit editing
    cout << COLOR_GREEN << "Book details updated successfully." << COLOR_RESET << endl; 
}

// Method to remove a book from the library by its title
// Removes a book from the catalog by its title after confirming with the user.
void LCMS::removeBook(string bookTitle, Book*& removedBook, string& removedBookCategory) {
    if (bookTitle == "") { // If no book title is provided
        cout << COLOR_RED << "No book title provided. Please enter a book title." << COLOR_RESET << endl; // Print an error message
        return; 
    }
    Node* StartNode= libTree->getRoot(); // Start from the root node
    Book* foundBook = libTree->findBook(StartNode, bookTitle); // Call the findBook method on the root node

    if (foundBook == nullptr) { // If the book is not found
        cout << COLOR_RED << "The book\" " << bookTitle << "\" not found." << COLOR_RESET << endl; // Print an error message
        return; 
    }

    char choice;
    cout << "Are you sure you want to remove " << bookTitle << "? (y/n): ";
    cin >> choice; // Read user choice
    cin.ignore(); // Ignore leftover '\n'

    if (choice == 'y' || choice == 'Y') { // If user confirms removal
        if (libTree->removeBook(StartNode, bookTitle, removedBook, removedBookCategory)) { // Call the removeBook method on the root node
        cout << COLOR_GREEN << "\"" << bookTitle << "\" has been deleted from the library catalog." << COLOR_RESET << endl; // Print a success message
        
        } else {
            cout << COLOR_RED << "Failed to remove " << bookTitle << "." << COLOR_RESET << endl; // Print an error message if removal fails
        }

    } else {
        cout << COLOR_RED << "Removal cancelled." << COLOR_RESET << endl; 
    }
}

// Method to find and display details of a specific category
void LCMS::findCategory(string category) {
    if (category == "") { // If no category name is provided
        cout << COLOR_RED << "No category name provided. Please enter a category name." << COLOR_RESET << endl; // Print an error message
        return; 
    }
    
    Node* categoryNode = libTree->getNode(category); // Get the category node from the tree

    if (categoryNode == nullptr) { // If the category node does not exist
        cout << COLOR_RED << " No such category found in the library catalog." << COLOR_RESET << endl; // Print an error message
        return; 
    }

    cout << COLOR_GREEN << "Category '" << categoryNode->name << "' was found in the library catalog." << COLOR_RESET << endl; // Print the name of the found category
}

// Method to add a new category to the library
void LCMS::addCategory(string category) {
    if (category == "") { // If no category name is provided
        cout << COLOR_RED << "No category name provided. Please enter a category name." << COLOR_RESET << endl; // Print an error message
        return; 
    }
    Node* categoryNode = libTree->getNode(category); // Get the category node from the tree

    if (categoryNode != nullptr) { // If the category node already exists
        cout << COLOR_RED << "Category " << category << " already exists in the catalog." << COLOR_RESET << endl; // Print an error message
        return; 
    }

    libTree->createNode(category); // Create a new category node in the tree
    cout << COLOR_GREEN << "Category " << category << " added successfully." << COLOR_RESET << endl; // Print a success message
}

// Method to edit the name of an existing category
void LCMS::editCategory(string category) {
    if (category == "") { // If no category name is provided
        cout << COLOR_RED << "No category name provided. Please enter a category name." << COLOR_RESET << endl; // Print an error message
        return; 
    }
    Node* categoryNode = libTree->getNode(category); // Get the category node from the tree

    if (categoryNode == nullptr) { // If the category node does not exist
        cout << COLOR_RED << "Category " << category << " not found." << COLOR_RESET << endl; // Print an error message
        return; 
    }

    string newCategoryName;
    do {
        cout << "Enter new category name: ";
        getline(cin, newCategoryName); // Read the new category name from user input
        if (newCategoryName.empty()) {
            cout << COLOR_RED << "Category name cannot be empty. Please enter a valid category name." << COLOR_RESET << endl;
        }
    } while (newCategoryName.empty()); // Prompt until a valid category name is provided

    categoryNode->name = newCategoryName; // Update the category name
    cout << COLOR_GREEN << "Category name updated successfully." << COLOR_RESET << endl; // Print a success message
}

// Method to remove a category (and its subcategories/books) from the library
void LCMS::removeCategory(string category) {
    if (category == "") { // If no category name is provided
        cout << COLOR_RED << "No category name provided. Please enter a category name." << COLOR_RESET << endl; // Print an error message
        return; 
    }
    Node* categoryNode = libTree->getNode(category); // Get the category node from the tree

    if (categoryNode == nullptr) { // If the category node does not exist
        cout << COLOR_RED << "Category " << category << " not found." << COLOR_RESET << endl; // Print an error message
        return; 
    }

    libTree->removeChild(categoryNode->parent, categoryNode->name); // Call the removeChild method on the parent node to remove the category
}
//==========================================================================
// End of the file

