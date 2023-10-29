#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <memory>

#include "Lexer.h"

enum class DataType
{
    NONE,
    UNINITIALIZED,
    BOOL,
    DOUBLE
};

class Node
{
public:
    Node() : value(Token(TokenType::NONE, "", -1, -1, -1)) {}
    Node(Token token) : value(token) {}
    virtual ~Node() = default;

    Token value;
};


// An expression node should always have 2 children
class ExpressionNode : public Node
{
public:
    ExpressionNode() : Node(), children() {}
    ExpressionNode(Token token) : Node(token), children() {}
    virtual ~ExpressionNode() = default;

    std::vector<std::unique_ptr<ExpressionNode>> children;
};


class WhileNode : public Node
{
public:
    WhileNode() : Node(), condition(std::make_unique<ExpressionNode>()), flows() {}
    WhileNode(Token token) : Node(token), condition(std::make_unique<ExpressionNode>()), flows() {}
    virtual ~WhileNode() = default;

    std::unique_ptr<ExpressionNode> condition;
    std::vector<std::unique_ptr<Node>> flows;    // expressions and statements inside {...}
};


class IfNode : public Node
{
public:
    IfNode() : Node(), conditions(), flowGroups() {}
    IfNode(Token token) : Node(token), conditions(), flowGroups() {}
    virtual ~IfNode() = default;

    std::vector<std::unique_ptr<ExpressionNode>> conditions;
    std::vector<std::vector<std::unique_ptr<Node>>> flowGroups;  // groups of expressions and statements inside each {...}
};


class PrintNode : public Node
{
public:
    PrintNode() : Node(), content(std::make_unique<ExpressionNode>()) {}
    PrintNode(Token token) : Node(token), content(std::make_unique<ExpressionNode>()) {}
    virtual ~PrintNode() = default;

    std::unique_ptr<ExpressionNode> content;
};


class ParserB
{
public:
    static std::pair<std::pair<int, int>, std::string> HandleTokenVector(std::vector<Token> tokenVector, int leftBound, int rightBound, std::vector<std::unique_ptr<Node>>& nodes);
    static std::pair<std::pair<int, int>, std::string> MakeExpressionTree(std::vector<Token> expression, int leftBound, int rightBound, std::unique_ptr<ExpressionNode>& node);
    static std::string calculate(Node* root, double& result, DataType& resultType);
    static void print(Node* root, int indent = 0);\
    static void printValue(double value, DataType valueType);
    static void setupExpression(std::vector<Token> expression);

    static std::map<std::string, double> variableMap;
    static std::map<std::string, bool> variableInitializedMap;
    static std::map<std::string, DataType> variableTypeMap;

    static std::map<TokenType, int> hierarchyMap;
private:
    static int findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightBracketNoError(std::vector<Token> expression, int leftBound, int rightBound);
};

