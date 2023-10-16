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


// exclude leftParenthesis
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

// right leftParenthesis
int findLeftParenthesis(std::vector<Token> expression, int leftBound, int rightBound)
{
    int balance = 1;
    int p = rightBound;
    while (p >= leftBound) {
        if (expression[p].type == TokenType::leftParenthesis) { balance -= 1; }
        if (expression[p].type == TokenType::rightParenthesis) { balance += 1; }
        if (balance == 0) { break; }
        p -= 1;
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
        // (+ 1 1 4)
        if (expression[leftBound].type == TokenType::leftParenthesis)
        {
            // ( only
            if (leftBound+1 > rightBound) {
                if (ErrorToken.type == TokenType::none) {
                    ErrorToken = expression[leftBound+1]; 
                }
                return Node();
            }
            // ( not followed by operation symbol
            if (expression[leftBound + 1].type != TokenType::plus && expression[leftBound + 1].type != TokenType::minus && expression[leftBound + 1].type != TokenType::multiply && expression[leftBound + 1].type != TokenType::divide)
            { 
                if (ErrorToken.type == TokenType::none) {
                    ErrorToken = expression[leftBound + 1]; 
                }
                return Node();
            }
            // right parentheses ) not found
            int rightIndex = findRightParenthesis(expression, leftBound + 1, rightBound);
            if (rightIndex > rightBound) 
            { 
                if (ErrorToken.type == TokenType::none) { 
                    ErrorToken = expression[rightIndex]; 
                }
                return Node();
            }
            return MakeTree(expression, leftBound + 1, rightIndex-1);
        }
            
        // number
        // 810
        else if (expression[leftBound].type == TokenType::number)
        {
            Node res = Node(expression[leftBound]);
            return res;
        }
            
        // operation
        // * 9 10 11
        else if (expression[leftBound].type == TokenType::plus || expression[leftBound].type == TokenType::minus || expression[leftBound].type == TokenType::multiply || expression[leftBound].type == TokenType::divide)
        {

            // nothing follows the symbol
            // *
            if (leftBound == rightBound)
            { 
                if (ErrorToken.type == TokenType::none) 
                { 
                    ErrorToken = expression[leftBound+1]; 
                } 
                return Node();
            }

            // iterate through all the elements and add them as children
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
                    // ( only
                    if (p+1 > rightBound) 
                    {
                        if (ErrorToken.type == TokenType::none) 
                        { 
                            ErrorToken = expression[p+1];
                        }
                        return Node();
                    }
                    // ( not followed by operation symbol
                    if (expression[p + 1].type != TokenType::plus && expression[p + 1].type != TokenType::minus && expression[p + 1].type != TokenType::multiply && expression[p + 1].type != TokenType::divide)
                    { 
                        if (ErrorToken.type == TokenType::none) 
                        { 
                            ErrorToken = expression[p+1]; 
                        } 
                        return Node();
                    }
                     
                    // ) not found
                    int rightIndex = findRightParenthesis(expression, p + 1, rightBound);
                    if (rightIndex > rightBound) 
                    { 
                        if (ErrorToken.type == TokenType::none) 
                        { 
                            ErrorToken = expression[rightIndex]; 
                        }
                        return Node();
                    }
                    res.children.push_back(MakeTree(expression, p, rightIndex));
                    p = rightIndex + 1;
                }
                    
                // ErrorToken
                // $
                else
                {
                    if (ErrorToken.type == TokenType::none) 
                    { 
                        ErrorToken = expression[p]; 
                    }
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
                    if (calculate(root.children[i]) == 0) { 
                        ErrorToken = Token(TokenType::error, "", -1, -1, -1);
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
            int v = floor(root.value.value); 
            // 1000.000  ->  1000
            if ((double)v == root.value.value) 
            { 
                std::cout << v; 
            }
            else 
            {
                std::cout << root.value.content;
            }
        } else {
            std::cout << "(";
            for (int i=0; i < (int)root.children.size(); i++)
            {
                print(root.children[i]);
                if (i != (int)root.children.size()-1)
                {
                    std::cout << " " << root.value.content << " ";
                }
            }
            std::cout << ")";
        }
    }
};

Token Parser::ErrorToken = Token(TokenType::none, "", -1, -1, -1);
