# Json Parser

A lightweight, custom JSON parser in C++. 
This university project implements a DOM-style JSON library capable of parsing (simple) JSON strings into a tree of C++ objects, manipulating them, and serializing them back into formatted text.

## Key Features

* **Complete Type Support:** Handles all standard JSON types: `null`, `boolean`, `number` (double), `string`, `list` (array), and `dictionary` (object).
* **Pimpl Idiom:** Utilizes the Pointer-to-Implementation (Pimpl) idiom to hide the internal memory management and data structures from the public API, ensuring fast compilation and a clean header (`json.hpp`).
* **Custom Iterators:** Implements custom forward iterators (`list_iterator`, `dictionary_iterator`, and their `const` variants) to allow standard C++ range-based loops and algorithms over JSON lists and dictionaries.
* **Stream Integration:** Fully overloads `operator<<` and `operator>>` for seamless integration with standard C++ input/output streams (`std::cin`, `std::cout`, file streams).
* **Memory Management:** Implements custom linked lists for arrays and key-value pairs, with robust deep-copying (Rule of Five) and memory cleanup to prevent leaks.

## Project Structure

The project consists of two main files:
* **`json.hpp`**: The header file containing the `json` class definition, custom iterator structs, and the `json_exception` definition.
* **`json.cpp`**: The source file containing the implementation details. It handles:
  * Type-checking and data modifiers (`set_string`, `set_list`, `push_back`, `insert`, etc.).
  * Overloaded stream operators for reading and writing.
  * Internal memory management and deep copying algorithms.

## Usage

To use this library in your own project, simply include the header and compile the source file alongside your code.

### 1. Include the header
```cpp
#include "json.hpp"
#include <iostream>

int main() {
    json my_json;
    
    // Example: parsing from standard input
    // std::cin >> my_json;
    
    // Example: creating a dictionary programmatically
    my_json.set_dictionary();
    json value;
    value.set_string("Hello World");
    my_json.insert({"greeting", value});
    
    // Output
    std::cout << my_json << std::endl;
    
    return 0;
}
```

### 2. Compilation

Compile your program along with json.cpp. For example, using GCC:
```bash
g++ -std=c++11 main.cpp json.cpp -o json_app
```
