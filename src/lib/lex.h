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
    Token(TokenType itype, std::string icontent, int iline, int iindex, float ivalue = -1);
    ~Token() = default;

    operator float() const {
        return value;
    }

public:
    static std::vector<Token> GenTokenVector(const std::string input);
    static void printLexer(const std::string input);
     
public:
    TokenType type;
    std::string content;
    int line = -1;
    int index = -1;
    float value = -1;

};