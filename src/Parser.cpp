#include "Parser.h"

Parser::Parser(const std::vector<std::string> expression) {
    Root = ParserHelper(expression, 0, nullptr);
}


// Node * Parser::ParserHelper(const std::vector<std::string> expression, int index, Node * root){
//     // std::cout<< "running" <<std::endl;
//     if (expression.at(index) == "(" && root == nullptr){
//         return ParserHelper(expression, index+1, root);
//     }

//     if (expression.at(index) == ")"){
        
//         return root;
//     }

//     if (expression.at(index) == "("){
//         Node * childrenR = ParserHelper(expression, index+1, nullptr);
//         root->Children.push_back(childrenR);
//         index = index + root->Children.size() + 1;

//     }

//     if (expression.at(index) == "+" || expression.at(index) == "-" || expression.at(index) == "*" || expression.at(index) == "/"){
//         if (root == nullptr){
//             root = new Node(expression.at(index));
//         }
//         else{
//             Node * ChildrenRoot = new Node(expression.at(index));
//             ChildrenRoot = ParserHelper(expression, index+1, ChildrenRoot);
//             root->Children.push_back(ChildrenRoot);
//         }

//     }

//     Node * curr = new Node(expression.at(index));
//     root->Children.push_back(curr);
//     ParserHelper(expression, index+1, root);

//     return root;
// }

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
            root->Children.push_back(curr)
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