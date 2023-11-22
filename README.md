# Calculator32
## Overview

Calculator32 is an ongoing project with the temporary goal of functioning as a calculator, with the ultimate aim of evolving into a lightweight script coding language. The project is structured into several main functions, each residing in the /src/ folder, with additional helper classes located in the /src/lib/ folder.

## Lexing (lex.cpp)

To compile the lexing component, use the following command:

```
g++ -Wall -Wextra -Werror -std=c++17 -DDEBUG=1 lex.cpp -o main lib/Lexer.cpp lib/ParserB.cpp
```

This program takes an input stringstream, processes the input string into a token vector, and prints their attributes. Add -DDEBUG=1 for debugging and -DDEBUG=0 otherwise.

## Calculator (calc.cpp)

Compile the calculator component with the following command:

g++ -Wall -Wextra -Werror -std=c++17 -DDEBUG=1 calc.cpp -o main lib/Lexer.cpp lib/ParserB.cpp

This program supports interactive infix input lines. It processes a line of input, prints the infix expression with parentheses, and displays the result.

## Formatting (format.cpp)

Compile the formatting component with:

g++ -Wall -Wextra -Werror -std=c++17 -DDEBUG=1 format.cpp -o main lib/Lexer.cpp lib/ParserB.cpp

This program takes an input stringstream, processes the input string into a tree, and prints the input in its standard format.

## Script Execution (scrypt.cpp)

Compile the script execution component with:

g++ -Wall -Wextra -Werror -std=c++17 -DDEBUG=1 scrypt.cpp -o main lib/Lexer.cpp lib/ParserB.cpp

This program takes an input stringstream and executes expressions and statements.



## Checkpoint Log
Checkpoint 1: Added lexer and parser. Support S expression calculations.

Checkpoint 2: Added support for variables, assignment operation, and infix expressions.

Checkpoint 3: Added support for Bool values, ordered comparisons, logic operations, mod operation, equality comparisons, while/if/print statements. S expression dumped.

Checkpoint 4: Added support for Functions and Arrays.


> William Lin
> 
> 
> Torry Tong
> 
> 
> Michael Yang
