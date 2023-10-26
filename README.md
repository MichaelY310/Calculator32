# Calculator32
This project is tempoparity a calculator, and final goal is to making a lightweight script coding language.

In checkpoint 2, we add the support of variables so that variables could be assigned a value and being calculated, and we also supported infix expressions so that we could read direct infix expressions rather than S expressions.

there are three main functions help with different functions.

# lex.cpp
Complie with Lexer.cpp and Utils.cpp, with lexer in Lexer.cpp and use Utils.cpp for debugging.

Turn the input string into a token vector and print their attributes

# parse.cpp
Complie with Lexer.cpp, Parser.cpp and Utils.cpp, with lexer in Lexer.cpp, parser for S-expression in Parser.cpp, and use Utils.cpp for debugging.

Turn the input string into a tree and then calculate the result of the expression.

# calc.cpp
Compile with Lexer.cpp, ParserB.cpp, and Utils.cpp, with lexer in Lexer.cpp, parser for infix expression in ParserB.cpp, and use Utils.cpp for debugging. 

Supports interactive infix input lines.



> William Lin
> Torry Tong
> Michael Yang
