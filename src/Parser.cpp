#include "lib/Parser.h"

Parser::Parser(const std::vector<std::string> expression) {
    Root = ParserHelper(expression, 0, nullptr);
}


Node * Parser::ParserHelper(const std::vector<std::string> expression, int index, Node * root){
    Node * curr = nullptr;
    // if (index > maxindex) {
    //     maxindex = index;    // used to check end
    // }
    if (expression.at(index) == ")"){ //7here
        // return root;
    }
    
    else if (expression.at(index) == "("){

        root = ParserHelper(expression, index + 1, root); //1 here /3here
    
        // return root;
    }

    else if (expression.at(index) == "+" || expression.at(index) == "-" || expression.at(index) == "*" || expression.at(index) == "/"){
        if (root == nullptr){
            root = new Node(expression.at(index));
            // std::cout << root->token << std::endl;
            curr = ParserHelper(expression, index + 1, root); //2 here
            // std::cout <<"CURR:" << curr->token << std::endl;
            // root->Children.push_back(curr);
            // return root;
        }
        else{
            curr = new Node(expression.at(index));
            curr = ParserHelper(expression, index + 1, curr); //4here (*)correct /
            root->Children.push_back(curr);
            // return root;
        }
    }
    else {
        curr = new Node(expression.at(index));
        root->Children.push_back(curr);
        ParserHelper(expression, index+1, root); //5here 6here 
        // return root;
    }




    return root;
}