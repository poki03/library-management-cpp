# Library Management System (C++)

A console application to manage books and magazines in a small library.

## Features
- Add books and magazines
- List all items and search by title
- Issue and return items (with error handling for unknown IDs and unavailable items)
- Data is saved to `library.csv` and loaded again when the program starts

## Concepts used
- Classes, inheritance and polymorphism (`LibraryItem` -> `Book`, `Magazine`)
- STL `std::map` with smart pointers
- Custom exceptions (`LibraryError`, `ItemNotFound`, `ItemUnavailable`)
- File handling with CSV

## Compile and run
```
g++ -std=c++17 library.cpp -o library
./library
```
On Windows use `library.exe` (or just `library`) to run it.

## Files
- `library.cpp` - source code
- `library.csv` - sample data
- `uml.png` - UML class diagram
- `output.png` - sample program output
