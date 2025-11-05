# String-Based C++ Calculator

This is a C++ program that implements string-based arithmetic operations in C++ without converting strings to built-in numeric types

## Features

* **File Input:** Reads pairs of numbers from a user-specified file.
* **String-Only Arithmetic:** Implements string-only addition, handling positive/negative numbers and decimal alignment.
* **Custom Validation:** Validates each number string with strict formatting rules (`(+|-)A(.B)`) without using `try-catch` or standard conversion functions.
* **Error Handling:** Detects and reports bad file and invalid number formats.

## How to Run

1.  Open your terminal and navigate to the `lab10` directory.
2.  Run the `make` command:

    ```bash
    make
    ```

3.  This will use the `g++` compiler (with `-std=c++11` and `-Wall`) to build the executable file named `calculator`.

Once built, you can run the program from your terminal.

4.  Execute the program:

    ```bash
    ./calculator
    ```

5.  The program will then prompt you to enter the name of the file containing the test cases (e.g., `input.txt`).

### Example Usage

```bash
$ make
g++ -std=c++11 -Wall -g -o calculator main.cpp

$ ./calculator
Enter a filename: input.txt
Line 1: 123 + 456 = 579
Line 2: 123.456 + +1.0 = 124.456
Line 3: +123 + -123 = 0
Line 4: -50.5 + -100.25 = -150.75
Line 5: Error - Invalid number.
  '-5.' is invalid.
...