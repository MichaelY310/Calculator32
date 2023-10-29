# Calculator32
This project is temporarity a calculator, and final goal is to making a lightweight script coding language.

In checkpoint 2, we added the support of variables so that variables could be assigned a value and being calculated, and we also supported infix expressions so that we could read direct infix expressions rather than S expressions.

We have all of the functions with main() functions in /src/ folder, with other classes that designed as helper (i.e. without main functions) in /src/lib/ folder.

there are three main functions help with different functions needed for the calculator.

# lex.cpp
Complie with Lexer.cpp and Utils.cpp, with lexer in Lexer.cpp and use Utils.cpp for debugging.

This program takes an input stringstrean, and then turn the input string into a token vector and print their attributes.

# parse.cpp
Complie with Lexer.cpp, Parser.cpp and Utils.cpp, with lexer in Lexer.cpp, parser for S-expression in Parser.cpp, and use Utils.cpp for debugging.

This program takes an input stringstream, and then turn the input string into a tree and then calculate the result of the expression, it will print the S-expression tree and result.

# calc.cpp
Compile with Lexer.cpp, ParserB.cpp, and Utils.cpp, with lexer in Lexer.cpp, parser for infix expression in ParserB.cpp, and use Utils.cpp for debugging. 

This program supports interactive infix input lines, taking a line of input each time and print the infix expression with parenthesis and result.


> William Lin
> 
> 
> Torry Tong
> 
> 
> Michael Yang
