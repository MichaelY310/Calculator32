#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <memory>
#include <stack>

#include "Lexer.h"

enum class DataType
{
    NONE,
    NUL,
    UNINITIALIZED,
    BOOL,
    DOUBLE,
    FUNCTION,
};

class Node
{
public:
    Node() : value(Token(TokenType::NONE, "", -1, -1, -1)) {}
    Node(Token token) : value(token) {}
    virtual ~Node() = default;

    Token value;
};

// An operation expression node should always have 2 children
// A function call expression: Node1: function. Node2, 3...: parameters. 
// A function call expression must have a variable token and children.size() != 0
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

class ReturnNode : public Node
{
public:
    ReturnNode() : Node(), content(std::make_unique<ExpressionNode>()) {}
    ReturnNode(Token token) : Node(token), content(std::make_unique<ExpressionNode>()) {}
    virtual ~ReturnNode() = default;

    std::unique_ptr<ExpressionNode> content;
};

class FunctionDefineNode : public Node
{
public:
    FunctionDefineNode() : Node(), functionName(), parameterNames(), flows() {}
    FunctionDefineNode(Token token) : Node(token), functionName(), parameterNames(), flows() {}
    virtual ~FunctionDefineNode() = default;

    std::string functionName;
    std::vector<std::string> parameterNames;
    std::vector<std::unique_ptr<Node>> flows;
};

class Function
{
public:
    Function() = default;
    std::vector<std::string> m_ParameterNames;
    std::vector<std::unique_ptr<Node>> m_FunctionFlows;
};

class Scope
{
public:
    Scope()
    : variableTypeMap(), variableDoubleMap(), variableBoolMap()
    , variableFunctionMap()
    {}

    // Copy constructor
    Scope(const Scope& other)
    : variableTypeMap(other.variableTypeMap), variableDoubleMap(other.variableDoubleMap),
        variableBoolMap(other.variableBoolMap)
        , variableFunctionMap(other.variableFunctionMap) 
    {}

    std::map<std::string, DataType> variableTypeMap;
    std::map<std::string, double> variableDoubleMap;
    std::map<std::string, bool> variableBoolMap;
    std::map<std::string, std::shared_ptr<Function>> variableFunctionMap;
};

class Result {
public:
    ~Result() {
        if (type == DataType::FUNCTION && function != nullptr) {
            // delete function;
            // function = nullptr;
        }
    }

    DataType type;
    union {
        double doubleValue;
        bool boolValue;
        Function* function;
    };
};

class ParserB
{
public:
    static std::pair<std::pair<int, int>, std::string> HandleTokenVector(std::vector<Token> tokenVector, int leftBound, int rightBound, std::vector<std::unique_ptr<Node>>& nodes);
    static std::pair<std::pair<int, int>, std::string> MakeExpressionTree(std::vector<Token> expression, int leftBound, int rightBound, std::unique_ptr<ExpressionNode>& node);
    static std::string calculate(Node* root, Result& result);
    static void print(Node* root, int indent = 0);
    static void printValue(Result& result);
    static void setupExpression(std::vector<Token>& expression);
    static void getVariable(std::string& variableName, Result& result);
    static void setVariable(std::string& variableName, Result& result);
    static int findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightBracketNoError(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightBraceNoError(std::vector<Token> expression, int leftBound, int rightBound);

    static std::stack<Scope> ScopeStack;
    static std::map<TokenType, int> hierarchyMap;
private:
};

