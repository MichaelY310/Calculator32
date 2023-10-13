#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

enum class TokenType {
    number,
    plus,
    minus,
    multiply,
    divide,
    leftParenthesis,
    rightParenthesis,
    end,
    error
};

class Token {

public:
    Token(TokenType itype, std::string icontent, int iline, int iindex, double ivalue = -1);
    ~Token() = default;

    operator double() const {
        return value;
    }

public:
    static std::vector<Token> GenTokenVector(const std::string input);  // returns a vector of tokens created from a string.
    static void printLexer(std::vector<Token> TokenVector);
    static void printLexer(const std::string input);
     
public:
    TokenType type;
    std::string content;    // value as a string
    int line = -1;
    int index = -1;
    double value = -1;       // a number has its own value. otherwise -1

};

