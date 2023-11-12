#include "Lexer.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <memory>
#include <stack>


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

class ArrayNode : public Node
{
    public:
    ArrayNode() : Node() {};
    ArrayNode(Token token) : Node(token) {};
    virtual ~ArrayNode() = default;

    std::vector<std::unique_ptr<Node>> ArrayContent;  // store array elements
};


int main() {
    std::unique_ptr<Node> A;
    std::unique_ptr<ArrayNode> B;
    std::unique_ptr<ExpressionNode> C;
    // std::unique_ptr<WhileNode> D;

    std::vector<std::unique_ptr<Node>> TEST;
    TEST.push_back(A);
    TEST.push_back(std::move(B));
    TEST.push_back(std::move(C));
    // TEST.push_back(D);
}
