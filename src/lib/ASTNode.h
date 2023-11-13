#pragma once


#include <memory>

#include "Lexer.h"

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
    
    bool ArrayLookUp = false;
    int index = -1;
    std::vector<std::unique_ptr<ExpressionNode>> children;
};


class ArrayNode : public Node
{
    public:
    ArrayNode() : Node(),  ArrayContent() {};
    ArrayNode(Token token) : Node(token),  ArrayContent() {};
    virtual ~ArrayNode() = default;

    bool lookUp = false;
    std::string lookUpStr = "";
    int lookUpIndex = -1;
    std::vector<std::unique_ptr<Node>> ArrayContent;  // stored all elements in array
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
    PrintNode() : Node(), content(std::make_unique<ExpressionNode>()), content2(nullptr) {}
    PrintNode(Token token) : Node(token), content(std::make_unique<ExpressionNode>()), content2(nullptr) {}
    virtual ~PrintNode() = default;

    std::unique_ptr<ExpressionNode> content;
    std::unique_ptr<ArrayNode> content2;
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

