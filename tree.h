#ifndef _TREE_H
#define _TREE_H

#include <string>
#include<algorithm>
#include "myvector.h"
#include "book.h"

using namespace std;

// Class representing a Node in the Tree
class Node 
{
	private:
	    string name;                // Name of the Node (represents a category or subcategories)
	    MyVector<Node*> children;   // List of child Nodes (subcategories)
	    MyVector<Book*> books;      // List of books stored in this Node
	    unsigned int bookCount;     // Count of books in this Node (Category) and its all subcategories
	    Node* parent;               // Pointer to the parent Node (nullptr for the root)

	public:
	    // Constructor to initialize a Node with a given name
	    Node(string name);

	    // Method to get the category name of the Node
	    string getCategory(Node* node);

	    // Destructor to clean up dynamically allocated memory
	    ~Node();

	    // Friend classes to allow access to private members
	    friend class Tree;
	    friend class LCMS;
};

//==========================================================
// Class representing a Tree structure
class Tree 
{
	private:
	    Node* root;  // Pointer to the root Node of the Tree

	public:
	    // Constructor to initialize the Tree with a root Node (Library)
	    Tree(string rootName);

	    // Destructor to clean up the entire Tree
	    ~Tree();

	    // Method to get the root Node of the Tree
	    Node* getRoot();

	    // Method to print the entire Tree structure
	    void print();

	private:
	    // Helper method to recursively print the Tree structure
	    void print_helper(string padding, string pointer, Node* node);

	    // Helper method to check if a Node is the last child of its parent
	    bool isLastChild(Node* ptr);

	public:
	    // Method to insert a new Node as a child of the given Node
	    void insert(Node* node, string name);

	    // Method to remove a child Node with the given name from the given node
	    void removeChild(Node* node, string child_name);

	    // Method to check if a Node is the root of the Tree
	    bool isRoot(Node* node);

	    // Method to get a Node based on its given path
	    Node* getNode(string path);

	    // Method to create a new Node at the given path
	    Node* createNode(string path);

	    // Method to get a child Node by its name
	    Node* getChild(Node* ptr, string childname);

	    // A recursive method to update the book count in a Node(subcategory) and its parent Nodes by a given offset
	    void updateBookCount(Node* ptr, int offset);

	    // Method to find a book by its title in a Node
	    Book* findBook(Node* node, string bookTitle);


	    // Method to remove a book by its title from a Node
	    bool removeBook(Node* node, string bookTitle, Book*& removedBook, string& removedBookCategory);

	    // A recursive method to print all books in a Node (category/subcategory) and its children
	    void printAll(Node* node);

	    // A recursive method to find all books (containg the keyword in its title or author) and categories that containg the keyword in its name
		void find(Node *node, string keyword, MyVector<Node*> &categoriesFound,MyVector<Book*> &booksFound);

	    // A recursive method to export data from a Node (category/subcategory) and its children to a file
	    int exportData(Node* node, ofstream& file);

	    // Method to check if the Tree is empty
	    bool isEmpty();
};

// Function to compute the Levenshtein distance between two strings
int levenshtein_distance(const std::string &s1, const std::string &s2);

#endif