#include <vector>
#include <iostream>
#include "Lexer.h"
#include "ParserB.h"
#include <string>

int main(){

    std::vector<Token> A;
    //Token(TokenType itype, std::string icontent, int iline, int iindex, double ivalue = -1)

    // A.push_back(Token(TokenType::variable, "x", 1, 0));
    // A.push_back(Token(TokenType::equals, "=", 1, 1));
    // A.push_back(Token(TokenType::variable, "y", 1, 2));
    // A.push_back(Token(TokenType::equals, "=", 1, 3));
    // A.push_back(Token(TokenType::number, "0", 1, 4));
    // A.push_back(Token(TokenType::plus, "+", 1, 5));
    // A.push_back(Token(TokenType::number, "1", 1, 6));
    // A.push_back(Token(TokenType::plus, "+", 1, 7));
    // A.push_back(Token(TokenType::number, "2", 1, 8));
    // A.push_back(Token(TokenType::multiply, "*", 1, 9));
    // A.push_back(Token(TokenType::number, "3", 1, 10));
    // A.push_back(Token(TokenType::minus, "-", 1, 11));
    // A.push_back(Token(TokenType::number, "4", 1, 12));
    // A.push_back(Token(TokenType::divide, "/", 1, 13));
    // A.push_back(Token(TokenType::leftParenthesis, "(", 1, 14));
    // A.push_back(Token(TokenType::number, "5", 1, 15));
    // A.push_back(Token(TokenType::plus, "+", 1, 16));
    // A.push_back(Token(TokenType::number, "6", 1, 17));
    // A.push_back(Token(TokenType::rightParenthesis, ")", 1, 18));
    // A.push_back(Token(TokenType::end, "", 1, 19));

// (7 - (b = (b + 5)))
    A.push_back(Token(TokenType::variable, "x", 1, 0));
    A.push_back(Token(TokenType::equals, "=", 1, 1));
    A.push_back(Token(TokenType::variable, "(", 1, 2));
    A.push_back(Token(TokenType::equals, "-", 1, 3));
    A.push_back(Token(TokenType::number, "(", 1, 4));
    A.push_back(Token(TokenType::plus, "b", 1, 5));
    A.push_back(Token(TokenType::number, "1", 1, 6));
    A.push_back(Token(TokenType::plus, "+", 1, 7));
    A.push_back(Token(TokenType::number, "2", 1, 8));
    A.push_back(Token(TokenType::multiply, "*", 1, 9));
    A.push_back(Token(TokenType::number, "3", 1, 10));
    A.push_back(Token(TokenType::minus, "-", 1, 11));
    A.push_back(Token(TokenType::number, "4", 1, 12));
    A.push_back(Token(TokenType::divide, "/", 1, 13));



    ParserB test = ParserB(A);
    std::cout << "root: " << test.mRoot->value.content << std::endl;
    std::cout << "First level: " << test.mRoot->leftchild->value.content << " "<< test.mRoot->rightchild->value.content <<std::endl;
    std::cout << "Second level: " << test.mRoot->rightchild->leftchild->value.content << " "<< test.mRoot->rightchild->rightchild->value.content <<std::endl;
    std::cout << "Third level: " << test.mRoot->rightchild->rightchild->leftchild->value.content << " "<< test.mRoot->rightchild->rightchild->rightchild->value.content <<std::endl;
    Node * curr = test.mRoot->rightchild->rightchild->leftchild;  //"+"
    Node * temp = test.mRoot->rightchild->rightchild->rightchild; //"/"
    std::cout << "Fourth level: " << curr->leftchild->value.content << " "<< curr->rightchild->value.content << "   "<< temp->leftchild->value.content <<" " << temp->rightchild->value.content  << std::endl;
    Node * C = curr->rightchild; // "*"
    curr = curr->leftchild; // "+"
    temp = temp->rightchild; //"+"
    std::cout << "Fifth level: " << curr->leftchild->value.content << " "<< curr->rightchild->value.content << "   "<< C->leftchild->value.content << " "<< C->rightchild->value.content <<"  "<< temp->leftchild->value.content <<" " << temp->rightchild->value.content  << std::endl;
    
    curr = nullptr;
    temp = nullptr;
    C = nullptr;
}