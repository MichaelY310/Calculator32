#include "lib/Parser.h"

Parser::Parser(const std::vector<Token> expression) {
   
    Root = ParserHelper(expression, 0, nullptr);
}


Node * Parser::ParserHelper(const std::vector<Token> expression, int index, Node * root){
    Node * curr = nullptr;
    if (index >= maxindex) {
        maxindex = index;    // used to check end
    }
    if (expression.at(index).content == ")"){  // Base Case
        return root;            
    }
    
    else if (expression.at(index).content == "("){ //When the element is (

        root = ParserHelper(expression, index + 1, root);  

        if (maxindex < expression.size()-1){ // Check whether the function reach the end of the vector
            // std::cout << "running" << std::endl;

            curr = ParserHelper(expression, maxindex+1, root); 
        }
    
        return root;
    }

    else if (expression.at(index).content == "+" || expression.at(index).content == "-" || expression.at(index).content == "*" || expression.at(index).content == "/"){
        if (root == nullptr){
            root = new Node(expression.at(index).content);  // for the first Root
        
            curr = ParserHelper(expression, index + 1, root); 
        
            return root;
        }
        else{   
            curr = new Node(expression.at(index).content);  
            curr = ParserHelper(expression, index + 1, curr);
            root->Children.push_back(curr);
            return root;
        }
    }
    else {          // when the element is number 
        curr = new Node(expression.at(index).content);
        root->Children.push_back(curr);         
        ParserHelper(expression, index+1, root); 
        return root;
    }
}

void Parser::printinfix(){
    std::string result = PrintHelp(Root, "", 0);
    std::cout << result << std::endl;
}

std::string Parser::PrintHelp(Node * root, std::string ExpStr, int index){
    if (root == nullptr){
        std::cout << "Nothing in the root !" << std::endl;
        return ExpStr;
    }
    
    if (index == root->Children.size()-1) {  // base case 
        if (root->Children.at(index)->token == "+" || root->Children.at(index)->token == "-" || root->Children.at(index)->token == "*" || root->Children.at(index)->token == "/") {
            ExpStr += " " + root->token + " " + PrintHelp(root->Children.at(index), "", 0) + ")"; //2
            return ExpStr;
        }
        else{
            ExpStr += " " + root->token + " " + root->Children.at(index)->token + ")";
            return ExpStr;
        } 
    }
    else if (index == 0){       // start of the string
        if (root->Children.at(index)->token == "+" || root->Children.at(index)->token == "-" || root->Children.at(index)->token == "*" || root->Children.at(index)->token == "/") {
            ExpStr += "("  + PrintHelp(root->Children.at(index), "", 0);
            return ExpStr;
        }
        else{
            ExpStr += "(" + root->Children.at(index)->token + PrintHelp(root, "" , index + 1);  //1 3
            return ExpStr;
        }
        
    }
    else if (index >= root->Children.size()) {       // if out of range exit
        std::cout << "WRONG" << std::endl;
        exit(-1);
    }
    else {      // other situation when 
        if (root->Children.at(index)->token == "+" || root->Children.at(index)->token == "-" || root->Children.at(index)->token == "*" || root->Children.at(index)->token == "/") {
            ExpStr += " " + root->token + " " + PrintHelp(root->Children.at(index), "", 0);
            return ExpStr;
        }
        else{
            ExpStr += " " + root->token + " " + root->Children.at(index)->token + PrintHelp(root, "", index + 1); //4
            return ExpStr;
        }
    }

}


