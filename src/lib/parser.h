#ifndef PARSER_H
#define PARSER_H

// #include "lex.h"
#include <iostream>
#include <string>
#include <vector>

struct Node {
    std::string token;
    std::vector<Node *> Children;   //vector can store more than 2 children
    float num;
    int paraCheck;
    bool end;


    Node(std::string inputStr){
        token = inputStr;
        num = -1;  
        paraCheck = 0;   
        end = false;          //when the node is token its num will be -1
    };

    Node(float inputNum){
        token = "?";
        num = inputNum;  
        paraCheck = 0;       // when the node is number it will have no token.
        end = false;
    };
};

class Parser {
    public:
        Parser(const std::vector<std::string> );
        Node * ParserHelper(const std::vector<std::string>, int , Node * );
        ~Parser(){};
        int maxindex = 0;

    // private:
        Node * Root;

};




#endif 