#ifndef _BOOK_H
#define _BOOK_H

#include <string>
using std::string;
// Class representing a Book in the library system
class Book 
{
	private:
	   string title;              // Title of the book
	   string author;             // Author of the book
	   string isbn;               // ISBN (International Standard Book Number) of the book
	    int publication_year;           // Year the book was published

	public:
	    // Constructor to initialize a Book object with the provided details
	    Book(std::string title,string author,string isbn, int publication_year);

	    // Method to display the details of the book (e.g., title, author, ISBN, publication year)
	    void print();
		string getTitle() const; // Method to get the title of the book

	    // Friend classes to allow access to private members
	    friend class Tree;  // Allows the Tree class to access private members of Book
	    friend class Node;  // Allows the Node class to access private members of Book
	    friend class LCMS;  // Allows the LCMS class to access private members of Book
};

#endif