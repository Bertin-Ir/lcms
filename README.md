# Library Catalog Management System (LCMS)

## Overview

The Library Catalog Management System 
is a C++ application designed to efficiently organize and manage book collections 
using hierarchical categories. It implements custom data structures and algorithms to provide robust 
book management capabilities through an interactive command-line interface.
---

## Key features

### 🚀 **Hierarchical Category Tree**
- Organize the library into nested categories and subcategories, mirroring real-world library structures.
- Effortlessly manage thousands of books with a scalable, general tree architecture.

### 🔍 **Advanced Search Functionality**
- **Fuzzy Search:** Instantly find books even with typos, thanks to the Levenshtein distance algorithm.
- **Keyword Search:** Search across titles, authors, and categories for maximum flexibility.
- **Category-Specific Listings:** List all books within any category or subcategory.

### ⏪ **Undo with Command Pattern**
- Add/remove actions implemented as undoable command objects.
- Command history is managed using a **stack** data structure.
- **Undoable commands**:
  - `addBook`: Undo last addition.
  - `removeBook`: Undo last deletion.
- Focused on the most frequent and impactful operations.

## 🖥️ Command-Line Interface (CLI)

LCMS is fully operated through a simple and intuitive command-line interface. Users can perform actions like adding, removing, searching, listing, and undoing changes to the catalog by entering straightforward commands.

Each command is designed to be interactive and user-guided—most will prompt you for input or confirm actions when needed. The system ensures ease of use while maintaining control over the library structure.

For guidance on available commands and usage, simply follow the on-screen instructions or run the `help` command.

---

## Get Started

1. **Clone or download the project.**
2. **Build with `make`.**
3. **Run with `./lcms`.**

*Note:* If you don't have make or g++ installed on your system, first install them using your package manager:

---

**Created by Bertin**
