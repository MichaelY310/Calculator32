#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include "Lexer.h"

struct Node {
    std::string token;
    std::vector<Node *> Children;   //vector can store more than 2 children
    float num;

    Node(){
        token = "empty";
        num = -1;
        Children = {};
    }

    Node(Node &C){
        token = C.token;
        for (auto & ele : C.Children){
            Children.push_back(ele);
        }
        num = C.num;
    }
    ~Node(){
        
    }

    Node(std::string inputStr){
        Children = {};
        token = inputStr;
        num = -1;           //when the node is token its num will be -1
    };

    Node(float inputNum){
        token = "?";
        Children = {};
        num = inputNum;     // when the node is number it will have no token.
    };
};

class Parser {
    public:
        Parser(const std::vector<Token> );
        Node * ParserHelper(const std::vector<Token>, size_t , Node * );
        ~Parser(){
            deleteHelp(Root);
        };

        Parser(){
            Root = new Node();  
        };

        void deleteHelp(Node * root);

        

        size_t maxindex = 0;

        void printinfix();

        std::string PrintHelp(Node * , std::string, size_t);
        double evaluateExpression();


    // private:
        Node * Root;
        std::string FixExp = "";
        double applyOp(char op, double b, double a);

};




#endif 