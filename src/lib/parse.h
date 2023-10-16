#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "lex.h"


class Node
{
public:
    Node() : value(Token(TokenType::none, "", -1, -1, -1)) {}
    Node(Token token) : value(token) {}

    std::vector<Node> children;
    Token value;
};


int findRightParenthesis(std::vector<Token> expression, int leftBound, int rightBound)
{
    int balance = 1;
    int p = leftBound;
    while (p <= rightBound) {
        if (expression[p].type == TokenType::leftParenthesis) { balance += 1; }
        if (expression[p].type == TokenType::rightParenthesis) { balance -= 1; }
        if (balance == 0) { break; }
        p += 1;
    }
    return p;
}


class Parser
{
public:
    static Token ErrorToken;
    
    static Node MakeTree(std::vector<Token> expression, int leftBound, int rightBound)
    {
        // (
        if (expression[leftBound].type == TokenType::leftParenthesis)
        {
            int rightIndex = findRightParenthesis(expression, leftBound + 1, rightBound);
            if (rightIndex > rightBound) 
            { 
                std::cout << "1" << std::endl;
                if (ErrorToken.type == TokenType::none) { ErrorToken = expression[rightIndex]; }
                return Node();
            }
            return MakeTree(expression, leftBound + 1, rightIndex-1);
        }
        // number
        else if (expression[leftBound].type == TokenType::number)
        {
            Node res = Node(expression[leftBound]);
            return res;
        }
        // operation
        else if (expression[leftBound].type == TokenType::plus || expression[leftBound].type == TokenType::minus || expression[leftBound].type == TokenType::multiply || expression[leftBound].type == TokenType::divide)
        {
            Node res = Node(expression[leftBound]);
            int p = leftBound + 1;
            while (p <= rightBound)
            {
                // number 
                if (expression[p].type == TokenType::number) 
                { 
                    res.children.push_back(MakeTree(expression, p, p)); 
                    p += 1;
                }
                // (
                else if (expression[p].type == TokenType::leftParenthesis) 
                { 
                    int rightIndex = findRightParenthesis(expression, p + 1, rightBound);
                    if (rightIndex > rightBound) 
                    { 
                        std::cout << "2" << std::endl;
                        std::cout << expression[p].index << std::endl;
                        if (ErrorToken.type == TokenType::none) { ErrorToken = expression[rightIndex]; }
                        return Node();
                    }
                    res.children.push_back(MakeTree(expression, p, rightIndex));
                    p = rightIndex + 1;
                }
                // ErrorToken
                else
                {
                    std::cout << "3" << std::endl;
                    if (ErrorToken.type == TokenType::none) { ErrorToken = expression[p]; }
                    return Node();
                }
            }
            return res;
        }
        return Node();
    }
    
    static double calculate(Node root) 
    {
        if (root.value.type == TokenType::number)
        {
            return root.value.value;
        } else {
            double res = calculate(root.children[0]);
            for (int i=1; i < (int)root.children.size(); i++)
            {
                if (root.value.type == TokenType::plus) { res += calculate(root.children[i]); }
                if (root.value.type == TokenType::minus) { res -= calculate(root.children[i]); }
                if (root.value.type == TokenType::multiply) { res *= calculate(root.children[i]); }
                if (root.value.type == TokenType::divide) 
                { 
                    if (root.children[i].value.value == 0) { 
                        ErrorToken = root.children[i].value;
                        return 1; 
                    }
                    res /= calculate(root.children[i]); 
                }
            }
            return res;
        }
    }
    
    static void print(Node root) 
    {
        if (root.value.type == TokenType::number)
        {
            std::cout << root.value.content << std::endl;
        } else {
            std::cout << root.value.content << std::endl;
    
            for (int i=0; i < (int)root.children.size(); i++)
            {
                print(root.children[i]);
            }
        }
    }
};

Token Parser::ErrorToken = Token(TokenType::none, "", -1, -1, -1);
