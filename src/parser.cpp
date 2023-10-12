#include "lib/parser.h"

Parser::Parser(const std::vector<std::string> expression) {
    Root = ParserHelper(expression, 0, nullptr);
}


Node * Parser::ParserHelper(const std::vector<std::string> expression, int index, Node * root){
    Node * curr = nullptr;
    if (index >= maxindex) {
        maxindex = index;    // used to check end
    }
    if (expression.at(index) == ")"){  // Base Case
        return root;            
    }
    
    else if (expression.at(index) == "("){ //When the element is (

        root = ParserHelper(expression, index + 1, root);  

        if (maxindex < expression.size()-1){ // Check whether the function reach the end of the vector
            // std::cout << "running" << std::endl;

            curr = ParserHelper(expression, maxindex+1, root); 
        }
    
        return root;
    }

    else if (expression.at(index) == "+" || expression.at(index) == "-" || expression.at(index) == "*" || expression.at(index) == "/"){
        if (root == nullptr){
            root = new Node(expression.at(index));  // for the first Root
        
            curr = ParserHelper(expression, index + 1, root); 
        
            return root;
        }
        else{   
            curr = new Node(expression.at(index));  
            curr = ParserHelper(expression, index + 1, curr);
            root->Children.push_back(curr);
            return root;
        }
    }
    else {          // when the element is number 
        curr = new Node(expression.at(index));
        root->Children.push_back(curr);         
        ParserHelper(expression, index+1, root); 
        return root;
    }

  
}