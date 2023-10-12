#include "lib/Parser.h"

Parser::Parser(const std::vector<std::string> expression) {
    Root = ParserHelper(expression, 0, nullptr);
}


Node * Parser::ParserHelper(const std::vector<std::string> expression, int index, Node * root){
    Node * curr = nullptr;
    if (expression.at(index) == ")"){
        return root;
    }
    
    if (expression.at(index) == "("){
        root = ParserHelper(expression, index + 1, root); // 1st here /4th here
        return root;
    }

    if (expression.at(index) == "+" || expression.at(index) == "-" || expression.at(index) == "*" || expression.at(index) == "/"){
        if (root == nullptr){
            root = new Node(expression.at(index));
            curr = ParserHelper(expression, index + 1, root); //2nd here
            root->Children.push_back(curr);
            return root;
        }
        else{
            curr = new Node(expression.at(index));
            curr = ParserHelper(expression, index + 1, curr); //5th here
            root->Children.push_back(curr);
            return root;
        }
    }
    else{
        curr = new Node(expression.at(index));
        root->Children.push_back(curr);
        ParserHelper(expression, index+1, root); //3rd here //6h here /7th here
        return root;
    }
}