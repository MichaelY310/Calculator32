#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <map>

#include "Lexer.h"
#include "Utils.h"


class Node
{
public:
    Node() : value(Token(TokenType::none, "", -1, -1, -1)) {}
    Node(Token token) : value(token) {}

    std::vector<Node> children;
    Token value;
};

class ParserB
{
public:
    static Node MakeTreeInfix(std::vector<Token> expression, int leftBound, int rightBound);
    static double calculate(Node root);
    static void print(Node root);
    static void setupExpression(std::vector<Token> expression);
    static void setupExpressionInfix(std::vector<Token> expression);
    static std::vector<std::vector<Token>> expressionLines;
    static std::map<std::string, double> variableMap;
    static std::map<std::string, bool> variableInitializedMap;
private:
    static int findLeftParenthesis(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightParenthesis(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound);
private:
    static std::map<TokenType, int> hierarchyMap;
};

