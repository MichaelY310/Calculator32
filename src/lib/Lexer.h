#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cmath>


enum class TokenType {
    NONE,               //
    VARIABLE,           // a b c
    NUMBER,             // 1 2 3

    LEFT_PARENTHESIS,    // (
    RIGHT_PARENTHESIS,   // )

    MULTIPLY,           // *
    DIVIDE,             // /
    MOD,                // %

    PLUS,               // +
    MINUS,              // -

    SMALLER,            // <
    BIGGER,             // >
    SMALLER_EQUAL,      // <=
    BIGGER_EQUAL,       // >=

    EQUALITY,           // ==
    INEQUALITY,         // !=

    AND,                // &

    EXCLUSIVE_OR,       // ^

    INCLUSIVE_OR,       // |

    ASSIGNMENT,             // =

    END,                // END
    ERROR,              // ERROR



    LEFT_BRACKET,    // {
    RIGHT_BRACKET,   // }
    TRUE,           
    FALSE,
    WHILE,
    IF,
    ELSE,
    PRINT

    ARRAY
};

class Token {

public:
    Token(TokenType itype, std::string icontent, int iline, int iindex, double ivalue = -1);
    ~Token() = default;

    operator double() const {
        return value;
    }

public:
    static std::pair<int, int> GenTokenVector(const std::string& input, std::vector<Token>& res);  // returns a vector of tokens created from a string.
    static void printLexer(std::vector<Token> TokenVector);
    static void printLexer(const std::string input);
     
public:
    TokenType type;
    std::string content;    // value as a string
    int line = -1;
    int index = -1;
    double value = -1;       // a number has its own value. otherwise -1

};
