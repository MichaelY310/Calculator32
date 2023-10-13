#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "lex.h"

struct Node {
    std::string token;
    std::vector<Node *> Children;   //vector can store more than 2 children
    float num;


    Node(std::string inputStr){
        token = inputStr;
        num = -1;           //when the node is token its num will be -1
    };

    Node(float inputNum){
        token = "?";
        num = inputNum;     // when the node is number it will have no token.
    };
};

class Parser {
    public:
        Parser(const std::vector<Token> );
        Node * ParserHelper(const std::vector<Token>, size_t , Node * );
        ~Parser(){};
        size_t maxindex = 0;

        void printinfix();

        std::string PrintHelp(Node * , std::string, size_t);
        double evaluateExpression();


    // private:
        Node * Root;
        std::string FixExp = "";
        double applyOp(char op, double b, double a);

};
