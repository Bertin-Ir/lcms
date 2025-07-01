#ifndef MYVECTOR_H
#define MYVECTOR_H

//============================================================================
// Name         : myvector.h
// Author       : Khalid Mengal
// Version      : 1.0
// Date Created : 16-03-2019
// Date Modified: 09-02-2025 
// Copyright    : All rights are reserved
// Description  : Vector implementation in C++
//============================================================================

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <stdexcept>
#include <sstream>

using namespace std;

// Template class for a custom vector implementation
template <typename T>
class MyVector 
{
	private:
	    T *array;                // Pointer to dynamically allocated array to store elements
	    int v_size;             // Current number of elements in the vector
	    int v_capacity;         // Total capacity of the vector (size of the allocated array)

	public:
	    // Constructors and Destructor
	    MyVector();                     // Default constructor (initializes an empty vector)
	    MyVector(int cap);              // Constructor with initial capacity
	    ~MyVector();                    // Destructor (deallocates memory)

	    // Vector Operations
	    void push_back(T element);      // Adds an element to the end of the vector
	    void reserve(int n);            //increase the capacity of the vector so that it can hold at least n elements
        void insert(int index, T element); // Inserts an element at a specific index
	    void erase(int index);          // Removes an element at a specific index
	    T& operator[](int index);       // Overloaded [] operator to access elements by index
	    T& at(int index);               // Access element at a specific index with bounds checking
	    const T& front();               // Returns a reference to the first element
	    const T& back();                // Returns a reference to the last element
	    int size() const;               // Returns the current number of elements in the vector
	    int capacity() const;           // Returns the current capacity of the vector
	    bool empty() const;             // Checks if the vector is empty
	    void shrink_to_fit();           // Reduces the vector's capacity to fit its size
	};

//==================================================================//
//	Please implement the required vector of Vector class below	//
//==================================================================//

// Vector Empty Exception class
class VectorEmpty : public runtime_error {
	public:
		VectorEmpty() : runtime_error("Vector is Empty") {}
};

//Constructor to initialize a vector with given capacity
template<typename T>
MyVector<T>::MyVector(int cap) : v_size(0), v_capacity(cap)
{
	array = new T[v_capacity];  //Allocate memory for array
}	

//Constructor to initialize an empty vector
template<typename T>
MyVector<T>::MyVector() : array(nullptr), v_size(0), v_capacity(0) {}	

//Destructor
template<typename T>
MyVector<T>::~MyVector()
{
	delete[] array;		//Deallocate memory
}

// This method returns current size of vector	
template<typename T>		
int MyVector<T>::size() const {
	return v_size;  //Return current size of vector
}

// This method returns capacity of vector
template<typename T>
int MyVector<T>::capacity() const {
	return v_capacity;  //Return capacity of vector
}

// This method increase the capacity of the vector so that it can hold at least n elements
template<typename T>
void MyVector<T>::reserve(int n) {
	if(n > v_capacity) {
		T *newArray = new T[n];  //Allocate memory for newArray array, with size n
		for(int i = 0; i < v_size; i++) {
			newArray[i] = array[i];  //Copy elements from array to newArray array
		}
		delete[] array;  //Delete the old array
		array = newArray;  //Assign newArray to array
		v_capacity = n;  //Update capacity of vector
	}
}	

// This method returns true if the vector is empty, False otherwise
template<typename T>
bool MyVector<T>::empty() const {
	return v_size == 0;  //Return true if the vector is empty, False otherwise
}

// This method returns reference of the first element in the vector
template<typename T>
const T& MyVector<T>::front() {
	if (empty()) {
		throw VectorEmpty();  //Throw VectorEmpty exception if the vector is empty
	}
	return array[0];  //Return reference of the first element in the vector
}

// This method returns reference of the Last element in the vector
template<typename T>
const T& MyVector<T>::back() {
	if (empty()) {
		throw VectorEmpty();  //Throw VectorEmpty exception if the vector is empty
	}
	return array[v_size - 1];  //Return reference of the Last element in the vector
}

// This method add an element at the end of vector
template<typename T>
void MyVector<T>::push_back(T element) {
	//Check if the vector is full
	if (v_size == v_capacity) {
		reserve(max(1, 2* v_capacity));  //Increase the capacity of the vector by 2 times the current capacity
	}
	array[v_size] = element;  //Add an element at the end of vector
	v_size++;  //Increment the size of vector
}

// This method add an element at the index
template<typename T>
void MyVector<T>::insert(int index, T element) {
	if (index < 0 || index >= v_size) {
		throw out_of_range("Vector index out of range...!");  //Throw out_of_range exception if the index is out of range
	}
	//Check if the vector is full
	if (v_size == v_capacity) {
		reserve(max(1, 2* v_capacity));  //Increase the capacity of the vector by 2 times the current capacity
	}
	for (int i = v_size-1; i >= index; i--) {
		array[i+1] = array[i];  //Shift elements to the right
	}
	array[index] = element;  //Add an element at the index
	v_size++;  //Increment the size of vector
}

// This method removes an element at the index from the vector
template<typename T>
void MyVector<T>::erase(int index) {
	if (index < 0 || index >= v_size) {
		throw out_of_range("Vector index out of range...!");  //Throw out_of_range exception if the index is out of range
	}
	for (int i = index; i < v_size-1; i++) {
		array[i] = array[i+1];  //Shift elements to the left
	}
	v_size--;  //Decrement the size of vector
}

// This method returns the reference of an element at given index
template<typename T>
T& MyVector<T>::operator[](int index) {
	return array[index];  //Return the reference of an element at given index
}

// This method returns the reference of the element at given index, it throws out_of_range exception if the index is out of range
template<typename T>
T& MyVector<T>::at(int index) {
	if (index < 0 || index >= v_size) {
		throw out_of_range("Vector index out of range...!");  //Throw out_of_range exception if the index is out of range
	}
	return array[index];  //Return the reference of the element at given index
}

// This method reduce vector capacity to fit its size
template<typename T>
void MyVector<T>::shrink_to_fit() {
	//Check if the size of vector is less than the capacity of vector
	if (v_size < v_capacity) {
		T *newArray = new T[v_size];  //Allocate memory for newArray array, with size v_size
		for(int i = 0; i < v_size; i++) {
			newArray[i] = array[i];  //Copy elements from array to newArray array
		}
		delete[] array;  //Delete the old array
		array = newArray;  //Assign newArray to array
		v_capacity = v_size;  //Update capacity of vector
	}
}
		
#endif