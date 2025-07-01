# Set g++ as the C++ compiler
CC=g++

# Enforce C++ 2011 as the language standard
CXXFLAGS=-std=c++11 

# Enable all standard warnings, uninitialized variable warnings, 
# and treat all warnings as errors
CXXFLAGS+= -Wall

# NOTE: comment the following line temporarily if 
# your development environment is failing
# due to these settings 
CXXFLAGS+=-fsanitize=address -fsanitize=undefined

# Object Files
OBJS=book.o tree.o lcms.o main.o 
# Target
TARGET=lcms

$(TARGET): $(OBJS)
	@echo "Linking: $(OBJS) -> $@"
	$(CC) $(CXXFLAGS) $(OBJS) -o $(TARGET)
book.o:	book.h book.cpp myvector.h
	@echo "Compiling: $^ -> $@"
	$(CC) $(CXXFLAGS) -c book.cpp
tree.o:	tree.h tree.cpp myvector.h
	@echo "Compiling: $^ -> $@"
	$(CC) $(CXXFLAGS) -c tree.cpp
lcms.o:	lcms.h lcms.cpp myvector.h
	@echo "Compiling: $^ -> $@"
	$(CC) $(CXXFLAGS) -c lcms.cpp		
# main.o:	main.cpp
# 	@echo "Compiling: $< -> $@"
# 	$(CC) $(CXXFLAGS) -c  main.cpp
main.o: main.cpp commandHistory.h addbook.h removebook.h icommand.h myvector.h
	@echo "Compiling: $^ -> $@"
	$(CC) $(CXXFLAGS) -c main.cpp
clean:
	@echo "Deleting: $(OBJS) $(TARGET)"
	rm -rf $(OBJS) $(TARGET)

# To compile the program, use the command:
# make
# To run the program, use the following command:
# ./lcms
# To clean up the build files, use the command:
# make clean

# End of makefile

