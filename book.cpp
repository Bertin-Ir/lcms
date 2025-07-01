// Name         : book.cpp
// Author       : Bertin Iradukunda
// Version      : 1.0
// Date Created : April 9, 2025
// Date Modified: April 9, 2025
// Description  : Book class describing a Book in the library system
//============================================================================

#include "book.h"
#include <iostream>
using namespace std;

// This constructor initializes the book's title, author, ISBN, and publication year
Book::Book(string title,string author,string isbn, int publication_year) 
{
    this->title = title;
    this->author = author;
    this->isbn = isbn;
    this->publication_year = publication_year;
}

// This method prints the details of the book, including title, author, ISBN, and publication year
void Book::print()
{
    cout << "Title: " << title << endl;
    cout << "Author: " << author << endl;
    cout << "ISBN: " << isbn << endl;
    cout << "Publication Year: " << publication_year << endl;
    cout << "----------------------------------------" << endl;
}
// This method returns the title of the book
string Book::getTitle() const
{
    return title; // Return the title of the book
}