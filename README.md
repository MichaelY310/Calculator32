# Calculator32

there are two main functions

# lex.cpp
Turn the input string into a token vector and print their attributes

# parse.cpp
Turn the input string into a tree and then calculate the result of the expression.


> William Lin
> Torry Tong
> Michael Yang

This project is temporarity a calculator, and final goal is to making a lightweight script coding language.

We have all of the functions with main() functions in /src/ folder, with other classes that designed as helper (i.e. without main functions) in /src/lib/ folder.

there are three main functions help with different functions needed for the calculator.

## lex.cpp
Complie with Lexer.cpp, with lexer in Lexer.cpp. Set -DDEBUG=1 for debugging and -DDEBUG=0 otherwise.

This program takes an input stringstream, and then turn the input string into a token vector and print their attributes.

## calc.cpp
Compile with Lexer.cpp, ParserB.cpp, with lexer in Lexer.cpp and parser for infix expression in ParserB.cpp. Set -DDEBUG=1 for debugging and -DDEBUG=0 otherwise.

This program supports interactive infix input lines, taking a line of input each time and print the infix expression with parenthesis and result.

## format.cpp
Complie with Lexer.cpp and ParserB.cpp, with lexer in Lexer.cpp and parser in Parser.cpp. Set -DDEBUG=1 for debugging and -DDEBUG=0 otherwise.

This program takes an input stringstream, and then turn the input string into a tree. It will print the input as its standard format.

## scrypt.cpp
Complie with Lexer.cpp and ParserB.cpp, with lexer in Lexer.cpp and parser in Parser.cpp. Set -DDEBUG=1 for debugging and -DDEBUG=0 otherwise.

This program takes an input stringstream and execute expressions and statements.



## Checkpoint Log
Checkpoint 1: Added lexer and parser. Support S expression calculations.

Checkpoint 2: Added support for variables, assignment operation, and infix expressions.

Checkpoint 3: Added support for Bool values, ordered comparisons, logic operations, mod operation, equality comparisons, while/if/print statements. S expression dumped.


> William Lin
> 
> 
> Torry Tong
> 
> 
> Michael Yang