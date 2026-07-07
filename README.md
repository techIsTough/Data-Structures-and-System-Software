# Data Structures and Algorithmic Implementations

This repository contains a collection of core system applications developed to explore foundational data structures and algorithm analysis. The projects transition from basic linear memory structures to complex, self-balancing hierarchical trees, with a strict emphasis on memory management, runtime efficiency ($O(\log n)$ performance boundaries), and pointer manipulation in C++.

## Project Collections

### 1. Arbitrary-Precision Linked List Calculator
* **Overview:** Built an arithmetic calculation engine capable of processing infinitely large integers that exceed the hardware storage limit of standard primitive types (`long long int`).
* **Implementation:** Implemented a doubly linked list structure where every node encapsulates a single digit of a number. Developed positional traversal algorithms to handle arithmetic carries, borrows, and sign changes for arbitrary-precision addition, subtraction, and multiplication.
* **Key Concept:** Linear node sequences, dynamic pointer reassignment, and manual deep-copy memory handling.

### 2. Hierarchical In-Memory File System
* **Overview:** Developed an interactive, mock Unix terminal terminal that simulates an in-memory directory hierarchy with standard system operations.
* **Implementation:** Engineered a multi-way tree structure where directory nodes manage collections of child pointers representing subdirectories or files. Programmed path parsing logic to traverse tree links and execute core operations like creating files (`touch`), making directories (`mkdir`), removing paths (`rm`), changing directories (`cd`), and printing the directory tree (`ls`).
* **Key Concept:** N-ary tree structures, recursive string path tokenization, and hierarchical tree traversal.

### 3. Self-Balancing AVL Database Engine
* **Overview:** Programmed a high-performance, key-value relational storage database designed for optimized record lookup, insertion, and deletion.
* **Implementation:** Constructed a robust AVL Tree from scratch. Wrote pointer-swapping logic for single and double tree rotations (Left, Right, Left-Right, Right-Left) to maintain optimal tree balance factors. Included database indexing commands to print sorted keys, count records within specific numeric ranges, and find nearest-match records.
* **Key Concept:** Self-balancing binary search trees, height-balanced verification, and logarithmic $O(\log n)$ search efficiency.

## Compilation and Execution

Each project includes a dedicated `makefile` configuration for simple building and compiling via standard Unix terminals.

### Setup & Compilation
Navigate into the respective project directory and compile the source code files using the GNU compiler collection:
```bash
make
