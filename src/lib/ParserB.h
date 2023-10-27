#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <stack>

#include "Lexer.h"


class Node
{
public:
    Node() : value(Token(TokenType::NONE, "", -1, -1, -1)) {}
    Node(Token token) : value(token) {}

    std::vector<Node> children;
    Token value;
};

class ParserB
{
public:
    static std::pair<std::pair<int, int>, std::string> MakeTreeInfix(std::vector<Token> expression, int leftBound, int rightBound, Node& node);
    static void parseLines(std::vector<std::string> expressionLines);
    static std::string calculate(Node root, double& result);
    static void print(Node root);
    static void setupExpression(std::vector<Token> expression);
    static void setupExpressionInfix(std::vector<Token> expression);
    static std::vector<std::vector<Token>> expressionLines;

    static std::stack<int> jumptoStack;
    static std::map<std::string, double> variableMap;
    static std::map<std::string, bool> variableInitializedMap;

    static std::map<TokenType, int> hierarchyMap;
private:
    static int findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightBracketNoError(std::vector<std::vector<Token>> TokenVectors, int leftBound, int rightBound);
};

