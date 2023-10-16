#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "Lexer.h"


class Node
{
public:
    Node() : value(Token(TokenType::none, "", -1, -1, -1)) {}
    Node(Token token) : value(token) {}

    std::vector<Node> children;
    Token value;
};


class Parser
{
public:
    static Token ErrorToken;
    
    static Node MakeTree(std::vector<Token> expression, int leftBound, int rightBound);
    static double calculate(Node root);
    static void testprint(Node root);
    static void print(Node root);

private:
    static int findRightParenthesis(std::vector<Token> expression, int leftBound, int rightBound);
};


