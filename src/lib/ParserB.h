#ifndef PARSERB_H
#define PARSERB_H

#include "Lexer.h"
#include <iostream>
#include <vector>
#include <string>


struct Node{ 
    Node() : value(Token(TokenType::none, "", -1, -1, -1)) {
        leftchild = nullptr;
        rightchild = nullptr;   
    }
    Node(Token token) : value(token) {
        leftchild = nullptr;
        rightchild = nullptr;
    }

    ~Node(){
        if(leftchild != nullptr && rightchild != nullptr){
            std::cout << "Node Deleting: " << leftchild->value.content << std::endl;
            std::cout << "Node Deleting: " << rightchild->value.content << std::endl;
            delete leftchild;
            delete rightchild;
            leftchild = nullptr;
            rightchild = nullptr;
        }
    }

    Node * leftchild;
    Node * rightchild;
    Token value;

};


class ParserB{

    public:
    ParserB(){
        mRoot = new Node();
        maxindex = 0;
    };
    ParserB(std::vector<Token> expression);
    ~ParserB(){

        if (mRoot != nullptr){
            std::cout << "Tree Deleting: " << mRoot->value.content << std::endl;
            delete mRoot;
            mRoot = nullptr;
        }
    };

    Node * Build(std::vector<Token> EXP, int index, Node * root);
    int precedence(Token input);
    int IsSigns(Token input);
    int nextSign(std::vector<Token> EXP, int index);
    int left_bound(std::vector<Token> EXP, int index);
    Node * Build_Parenthesis(std::vector<Token> EXP, int index, int leftBoud);


    Node * mRoot;
    int maxindex;  
    int level = 0;
};




#endif
