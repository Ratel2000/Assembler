# Assembly Language Assembler

This project is an implementation of an assembler for a custom assembly language. The assembler consists of several components, each responsible for different aspects of the assembly process.

## Project Components

### 1. First Pass

The `firstPass` module performs the initial pass through the assembly code, analyzing and extracting essential information needed for the assembly process. This includes identifying labels, directives, and instructions.

- **File:** `firstPass.c`, `firstPass.h`

### 2. Input Analyzer

The `inputAnalyze` module is responsible for analyzing the input assembly code. It handles tasks such as reading lines from the input file, checking syntax validity, and identifying the type of statements (empty, comment, directive, or instruction).

- **Files:** `inputAnalyze.c`, `inputAnalyze.h`

### 3. Label Table Linked List

The `labelTableLinkedList` module manages a linked list data structure to store information about labels encountered during the assembly process. This includes the label's type, address, and location.

- **Files:** `labelTableLinkedList.c`, `labelTableLinkedList.h`

### 4. Data Image

The `dataImage` module is responsible for managing the data image, which holds information about the assembled data in memory. This includes handling data directives and generating the final machine code.

- **Files:** `dataImage.c`, `dataImage.h`

### 5. Makefile

The `Makefile` provides rules for compiling the source code and linking the different components to create the final executable.

- **File:** `Makefile`

## Compilation and Execution

To compile the assembler, use the provided Makefile. Here's an example command:

```bash
make

