// Name         : tree.cpp
// Author       : Bertin Iradukunda
// Version      : 1.0
// Date Created : April 9, 2025
// Date Modified: June 21, 2025
// Description  : Node and Tree class tres representing a hierarchical structure of categories and books
//============================================================================

#include "tree.h"
#include "myvector.h"
#include <iostream>
#include <sstream> 
#include <fstream> 
#include <string>
#include <cctype>
#define FUZZY_SEARCH_THRESHOLD 2 // Define a threshold for fuzzy search
using namespace std;

// Constructor to initialize a Node with a given name
Node::Node(string name) 
{
    this->name = name;
    this->bookCount = 0; // Initialize book count to 0
    this->parent = nullptr; // Initialize parent to nullptr
}

// This method returns the category name of the Node
// It uses recursion to construct the full path. 
string Node::getCategory(Node* node) 
{
    string category = node->name; // Start with the current node's name

    // If the node has a parent, recursively get the parent's category
    if (node->parent != nullptr) 
    {
        category = node->parent->getCategory(node->parent) + "/" + category;
    }

    return category; // Return the full path of the category
}

// Destructor to clean up dynamically allocated memory
// It deletes all child nodes and books asstrociated with the current node
Node::~Node() 
{
    for (int i = 0; i < books.size(); i++) 
    {
        string bookTitle = books[i]->title; // Get the title of the book
        delete books[i]; // delete each book in the books vector
        cout << "Book \"" << bookTitle << "\" has been deleted from the library catalog." << endl;
    }
    for (int i = 0; i < children.size(); i++) 
    {
        delete children[i]; // recursively  delete each child node 
    }
    cout << "Category \"" << name << "\" has been deleted from the library catalog." << endl; 
    
}

// This method calculates the Levenshtein distance between two strings
// It takes two strings as input and returns the minimum number of single-character edits (insertions,
// deletions, or substitutions) required to change one string into the other
int levenshtein_distance(const string &s1, const string &s2) {
    int len_s1 = s1.length();
    int len_s2 = s2.length();

    // create a 2D vector to store distances
    MyVector<MyVector<int>> dist(len_s1 + 1);
    for (int i = 0; i <= len_s1; ++i) {
        dist[i].reserve(len_s2 + 1);   // Reserve space for each row
    }

    // Initialize the first row and column
    for (int i = 0; i <= len_s1; ++i) {
        dist[i][0] = i; // Deletion cost
    }
    for (int j = 0; j <= len_s2; ++j) {
        dist[0][j] = j; // Insertion cost
    }

    // Compute the distances
    for (int i = 1; i <= len_s1; ++i) {
        for (int j = 1; j <= len_s2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dist[i][j] = std::min({dist[i - 1][j] + 1,      // Deletion
                                   dist[i][j - 1] + 1,      // Insertion
                                   dist[i - 1][j - 1] + cost}); // Substitution
        }
    }

    return dist[len_s1][len_s2]; // Return the Levenshtein distance
}

// This helper method formats a string for CSV output
// It encloses the string in double quotes if it contains a comma(s) and returns the formatted string
string formatCSVString(const string& elem) {
    if (elem.find(',') != string::npos) {
        return "\"" + elem + "\""; // Enclose in double quotes if it contains a comma
    }
    return elem; // Return as it is if no comma found
}

// This helper method converts a string to lowercase
// It iterates through each character and converts it to lowercase and returns the new string
string toLowerCase(const string& str) {
    string lowerStr;
    for (char c : str) {  // Iterate through each character in the string
        lowerStr += tolower(c);
    }
    return lowerStr;
}

// Helper function to remove leading and rear edge whitespace
string strip_whitespace(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r"); // Find the first non-whitespace character
    size_t end = str.find_last_not_of(" \t\n\r"); // Find the last non-whitespace character
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1); // Return the substring without leading and rear edge whitespace, if any
}

// Constructor to initialize the Tree with a root Node (Library)
Tree::Tree(string rootName) {
    root = new Node(rootName); // Create the root node with the given name
}

// Destructor to clean up the entire Tree
Tree::~Tree() {
    delete root; // Delete the root node, which will recursively delete all its children and books
}

// this method returns the root Node of the Tree
Node* Tree::getRoot() {
    return root; // Return the root node of the tree
}

// This method prints the entire Tree structure
// It uses a helper function to recursively print the tree with proper formatting
void Tree::print() {
    print_helper("", "", root); // Call the helper function to print the tree
}

// This method checks if a Node is the last child of its parent
// It returns true if the node is the last child, otherwise false
bool Tree::isLastChild(Node *ptr)
{
    if(ptr!=root and ptr == ptr->parent->children[ptr->parent->children.size()-1])
        return true;
    return false;
}

// This method is a helper function that recursively prints the tree structure
void Tree::print_helper(string padding, string pointer, Node *node)
{
    if (node != nullptr) 
    {
        cout << padding << pointer << node->name << "(" << node->bookCount << ")" << endl;

        if (node != root) 
            padding += (isLastChild(node)) ? "   " : "│  ";

        for (int i = 0; i < node->children.size(); i++)
        {
            string marker = isLastChild(node->children[i]) ? "└──" : "├──";
            print_helper(padding, marker, node->children[i]);
        }
    }
}

// This method creates and inserts a new node with the given name as a child of the specified node
// It prints an error messtrage if a child/subcategory with the same name already exists in the current node
void Tree::insert(Node* node, string child_name) 
{
    // Check if the child name already exists in the current node
    for (int i = 0; i < node->children.size(); i++) 
    {
        if (node->children[i]->name == child_name) 
        {
            cout << child_name << "' already exists." << endl;
            return; // Exit if a child with the same name is found
        }
    }

    Node* newNode = new Node(child_name); // Create a new node with the given name
    newNode->parent = node; // Set the parent of the new node to the current node
    node->children.push_back(newNode); // Add the new node to the children vector
}

// This Method removes a child Node with the given name from the given node
void Tree::removeChild(Node* node, string child_name) {
    // Check if the child name exists in the current node
    for (int i = 0; i < node->children.size(); i++) 
    {
        if (node->children[i]->name == child_name) 
        {
            int Books_toremove = node->children[i]->bookCount; // Get the number of books to remove
            // Recursively delete the child node and all its descendants
            delete node->children[i]; 
            node->children.erase(i); // Remove the child from the parent's children vector

            // Update the book count of the parent node
            updateBookCount(node, -Books_toremove); 

            return; 
        }
    }
    cout << child_name << " not found." << endl; // Print an error messtrage if the child is not found
}

// This method checks if a Node is the root of the Tree
bool Tree::isRoot(Node* node) 
{
    return node->parent == nullptr ; // Return true if the node is the root, otherwise false
}

// This method gets a Node based on its given path
// It splits the path into categories and traverses the tree to find the corresponding node
// It returns the found node or nullptr if not found
Node* Tree::getNode(string path) 
{
    Node* currentNode = root; // Start from the root node
    string category; // Variable to store the current category

    // Split the path into categories and traverse the tree
    stringstream sstr(path);
    while (getline(sstr, category, '/')) 
    {
        if (category == "") continue; // Skip empty categories

        // Check if the child with the given name exists in the current node
        Node* child = getChild(currentNode, category); 
        if (child == nullptr) 
        {
            return nullptr; // Return nullptr if the category is not found
        }
        currentNode = child; // Move to the child node
    }

    return currentNode; // Return the found node
}

// This method creates a new Node at the given path
// It returns the created node 
Node* Tree::createNode(string path) 
{
    Node* currentNode = root; // Start from the root node
    string category; // Variable to store the current category

    // Split the path into categories and traverse/create nodes as needed
    stringstream sstr(path);
    while (getline(sstr, category, '/')) 
    {
        if (category == "") continue; // Skip empty categories

        // Check if the child with the given name exists in the current node
        Node* child = getChild(currentNode, category); 
        if (child == nullptr) 
        {
            // If the child does not exist, create it
            insert(currentNode, category);
            child = getChild(currentNode, category); // Get the newly created child
        }
        currentNode = child; // Move to the child node
    }
    return currentNode; // Return the created or found node
}

// This method finds and returns a child Node by its name
Node* Tree::getChild(Node* ptr, string childname) {

    string ToLowerChildName = toLowerCase(strip_whitespace(childname)); // Convert the child name to lowercase and strip whitespace
    // Check if the child name exists in the current node
    for (int i = 0; i < ptr->children.size(); i++) 
    {
        if (toLowerCase(strip_whitespace(ptr->children[i]->name)) == ToLowerChildName) // Make case insensitive comparison
        {
            return ptr->children[i]; // Return the found child node
        }
    }
    return nullptr; // Return nullptr if the child is not found
}

// This method updates the book count in a Node (subcategory) and its parent Nodes by a given offset
// It is used to keep track of the number of books in each category and subcategory
void Tree::updateBookCount(Node* ptr, int offset) {
    if (ptr == nullptr) 
    {
        return; // Return if the node is null
    }
    ptr->bookCount += offset; // Update the book count of the current node

    // Recursively update the book count of the parent node
    if (ptr->parent != nullptr) 
    {
        updateBookCount(ptr->parent, offset); // Call the method for the parent node
    }
}

// This method finds a book by its title in a Node
// It returns the found book and prints its detail or nullptr if not found
Book* Tree::findBook(Node* node, string bookTitle) 
{
    if (node == nullptr) 
    {
        return nullptr; // Return nullptr if the node is null
    }
    // Check if the book title is empty
    if (bookTitle == "") 
    {
        cout << "No title provided. Please enter a book title" << endl; 
        return nullptr; // Return nullptr if the book title is empty
    }
    string ToLowerBookTitle = toLowerCase(strip_whitespace(bookTitle)); // Convert the book title to lowercase and strip whitespace
    // Check if the book title exists in the current node's books vector
    for (int i = 0; i < node->books.size(); i++) 
    {
        string candidate = toLowerCase(strip_whitespace(node->books[i]->title));
        if (candidate == ToLowerBookTitle ||
            levenshtein_distance(candidate, ToLowerBookTitle) <= FUZZY_SEARCH_THRESHOLD) // Check for exact match or fuzzy match
        {
            cout << "Book found in the library: " << endl;
            cout << "-------------------------" << endl;
            node->books[i]->print(); // Print the details of the found book
            return node->books[i]; // Return the found book
        }
    }

    // Recursively search in the children nodes
    for (int i = 0; i < node->children.size(); i++) 
    {
        Book* foundBook = findBook(node->children[i], bookTitle); // Call the method for each child node
        if (foundBook != nullptr) 
        {
            return foundBook; // Return the found book if it exists in a child node
        }
    }
    return nullptr; // Return nullptr if the book is not found in the current node or its children
}


// This method removes a book by its title from a Node
// It returns true if the book is removed successfully, otherwise false
bool Tree::removeBook(Node* node, string bookTitle, Book*& removedBook, string& removedBookCategory) 
{
    if (node == nullptr) 
    {
        return false; // Return false if the node is null
    }
    // Check if the book title is empty
    if (bookTitle == "") 
    {
        cout << "No title provided. Please enter a book title" << endl; 
        return false; // Return false if the book title is empty
    }
    removedBook = nullptr; // Initialize the removedBook reference to nullptr
    string ToLowerBookTitle = toLowerCase(strip_whitespace(bookTitle)); // Convert the book title to lowercase
    // Check if the book title exists in the current node's books vector
    for (int i = 0; i < node->books.size(); i++) 
    {
        string candidate = toLowerCase(strip_whitespace(node->books[i]->title));
        if (candidate == ToLowerBookTitle ||
            levenshtein_distance(candidate, ToLowerBookTitle) <= FUZZY_SEARCH_THRESHOLD) // Check for exact match or fuzzy match
        {
            removedBook = node->books[i]; // Store the found book in the reference parameter
            removedBookCategory = node-> name; // Store the category of the found book
            // delete node->books[i]; // Delete the found book
            node->books.erase(i); // Remove the book from the books vector

            // Update the book count of the parent node
            updateBookCount(node, -1); 

            return true; // Return true if the book is removed successfully
        }
    }
    // Recursively search in the children nodes
    for (int i = 0; i < node->children.size(); i++) 
    {
        if (removeBook(node->children[i], bookTitle, removedBook, removedBookCategory)) // Call the method for each child node
        {
            return true; // Return true if the book is removed successfully from a child node
        }
    }
    return false; // Return false if the book is not found
}

// This method prints all books in a Node (category/subcategory) and its children
void Tree::printAll(Node* node) 
{
    // Print the details of each book in the current node
    for (int i = 0; i < node->books.size(); i++) 
    {
        node->books[i]->print(); // Print the details of the book
    }

    // Recursively print all books in the children nodes
    for (int i = 0; i < node->children.size(); i++) 
    {
        printAll(node->children[i]); // Call the method for each child node
    }
}

// A recursive method to find all books (containg the keyword in its title or author) and categories that containg the keyword in its name
// It is case insensitive
void Tree::find(Node *node, string keyword, MyVector<Node*> &categoriesFound, MyVector<Book*> &booksFound) 
{
    if (node == nullptr) 

    {
        return; // Return if the node is null
    }
    // Check if the keyword is empty
    if (keyword == "") 
    {
        cout << "No keyword provid. Please enter a keyword" << endl; 
        return; // Return if the keyword is empty
    }
    
    string lowerKeyword = toLowerCase(strip_whitespace(keyword)); // Convert the keyword to lowercase for case-insensitive comparison

    // Check if the node name contains the keyword or is a fuzzy match
    string nodeName = toLowerCase(strip_whitespace(node->name));
    if (nodeName.find(lowerKeyword) != string::npos ||
        levenshtein_distance(nodeName, lowerKeyword) <= FUZZY_SEARCH_THRESHOLD)
    {
        categoriesFound.push_back(node); // Add the node to the categories found vector
    }

    // Check if any book in the current node contains the keyword in its title or author, or is a fuzzy match
    for (int i = 0; i < node->books.size(); i++) 
    {
        string bookTitle = toLowerCase(strip_whitespace(node->books[i]->title));
        string bookAuthor = toLowerCase(strip_whitespace(node->books[i]->author));
        if (bookTitle.find(lowerKeyword) != string::npos ||
            bookAuthor.find(lowerKeyword) != string::npos ||
            levenshtein_distance(bookTitle, lowerKeyword) <= FUZZY_SEARCH_THRESHOLD ||
            levenshtein_distance(bookAuthor, lowerKeyword) <= FUZZY_SEARCH_THRESHOLD)
        {
            booksFound.push_back(node->books[i]); // Add the book to the books found vector
        }
    }

    // Recursively search in the children nodes
    for (int i = 0; i < node->children.size(); i++) 
    {
        find(node->children[i], keyword, categoriesFound, booksFound); // Call the method for each child node
    }
}


// A recursive method to export data from a Node (category/subcategory) and its children to a file
// It returns the number of books exported
int Tree::exportData(Node* node, ofstream& file) 
{
    if (node == nullptr) 
    {
        return -1; // Return -1 if the node is null
    }
    // Check if the file is open
    if (file.fail()) 
    {
        cerr << "File is not open." << endl;
        return -1; // Return -1 if the file is not open
    }
    
    int count = 0; // Initialize the count of books exported to 0

    // Write the details of each book in the current node to the file
    for (int i = 0; i < node->books.size(); i++) 
    {
        string category = node->getCategory(node);

        // Remove the root name from the category path
        if (category.find(root->name + "/") == 0) {
            category = category.substr(root->name.length() + 1);
        }
        // Format the book details for CSV output
        file << formatCSVString(node->books[i]->title) << "," 
             << formatCSVString(node->books[i]->author) << "," 
             << node->books[i]->isbn << "," 
             << node->books[i]->publication_year << "," 
             << category << endl;
        count++; // Increment the count of books exported
    }

    // Recursively export data from the children nodes
    for (int i = 0; i < node->children.size(); i++) 
    {
        count += exportData(node->children[i], file); // Call the method for each child node and add to the count
    }

    return count; // Return the total number of books exported
}

// This method checks if the Tree is empty
// It returns true if the root node has no children and no book, otherwise false
bool Tree::isEmpty() 
{
    // Check if the root has no children and no book
    return (root->children.size() == 0 && root->books.size() == 0);
}
//=============================================================================
// End of file