#include "lib/parse.h"
#include "lib/lex.h"
#include <stack>
#include <sstream>
#include <cctype>

Parser::Parser(const std::vector<Token> expression) {
   
    Root = ParserHelper(expression, 0, nullptr);
}


Node * Parser::ParserHelper(const std::vector<Token> expression, size_t index, Node * root){
    // std::cout << "running1" <<std::endl;
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
    // std::cout << "running" <<std::endl;
    FixExp = PrintHelp(Root, "", 0);
    std::cout << FixExp << std::endl;
}

std::string Parser::PrintHelp(Node * root, std::string ExpStr, size_t index){
    
    
    if (root == nullptr){
        std::cout << "Nothing in the root !" << std::endl;
        return ExpStr;
    }
    
    if (index == root->Children.size()-1) {  // base case 
        if (root->Children.at(index)->token == "+" || root->Children.at(index)->token == "-" || root->Children.at(index)->token == "*" || root->Children.at(index)->token == "/") {
            ExpStr += " " + root->token + " " + PrintHelp(root->Children.at(index), "", 0) + ")"; //
            return ExpStr;
        }
        else{
            ExpStr += " " + root->token + " " + root->Children.at(index)->token + ")";
            return ExpStr;
        } 
    }
    else if (index == 0){       // start of the string
        if (root->Children.at(index)->token == "+" || root->Children.at(index)->token == "-" || root->Children.at(index)->token == "*" || root->Children.at(index)->token == "/") {
            ExpStr += "("  + PrintHelp(root->Children.at(index), "", 0); //1 2 

            if (index < root->Children.size()-1) {
                ExpStr += PrintHelp(root, "", index + 1);
            }

            return ExpStr;
        }
        else{
            ExpStr += "(" + root->Children.at(index)->token + PrintHelp(root, "" , index + 1);  //3
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
            ExpStr += " " + root->token + " " + root->Children.at(index)->token + PrintHelp(root, "", index + 1); //
            return ExpStr;
        }
    }

}

double Parser::applyOp(char op, double b, double a) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0) {
            std::cout << "Runtime Error: Division by zero." << std::endl;
            exit(3);
        }
        return a / b;
    }
    return 0;
}

// Function to evaluate arithmetic expressions
double Parser::evaluateExpression() {
    std::istringstream iss(FixExp);
    std::stack<double> values;
    std::stack<char> ops;

    char current;
    while (iss >> current) {
        if (isdigit(current) || current == '.') {
            iss.putback(current);
            double value;
            iss >> value;
            values.push(value);
        }
        else if (current == '(') {
            ops.push(current);
        }
        else if (current == ')') {
            while (ops.top() != '(') {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(op, val2, val1));
            }
            ops.pop();
        }
        else if (current == '+' || current == '-' || current == '*' || current == '/') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(op, val2, val1));
            }
            ops.push(current);
        }
    }

    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(op, val2, val1));
    }

    return values.top();
}



int main() {

    std::string input;
    while (true) {
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            // Stop reading if the line is empty
            break;
        }

        // Concatenate the lines into the input string
        input += line + '\n';
    }

    std::vector<Token> TokenVector = Token::GenTokenVector(input);
    // Check ERROR

    std::vector<Token> T;
    for (size_t i = 0; i < TokenVector.size()-1; i++){
        T.push_back(TokenVector.at(i));
    }

    Parser p(T);
    p.printinfix();
    double result;

    result = p.evaluateExpression();
    std::cout << result << std::endl;


    return 0;

}
   