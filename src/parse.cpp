#include "lib/parse.h"

void errorCheck(std::vector<Token> expression) {
    if (expression.size() == 1 || !(expression.at(0).type == TokenType::leftParenthesis || expression.at(0).type == TokenType::number)) {
        std::cout << "Unexpected token at line " << expression.at(0).line << " column " << expression.at(0).index << ": " << expression.at(0).content << std::endl;
        exit(2);
    }
    if (expression.at(0).type == TokenType::number) {
        if (expression.size() != 2) {
            std::cout << "Unexpected token at line " << expression.at(1).line << " column " << expression.at(1).index << ": " << expression.at(1).content << std::endl;
            exit(2);
        }
        else {
            return;
        }
    }

    if (!(expression.at(1).type == TokenType::plus || expression.at(1).type == TokenType::minus || expression.at(1).type == TokenType::multiply || expression.at(1).type == TokenType::divide)) {
        std::cout << "Unexpected token at line " << expression.at(1).line << " column " << expression.at(1).index << ": " << expression.at(1).content << std::endl;
        exit(2);
    }

    int paraCheck = 1;
    for (size_t i = 1; i < expression.size() - 1; i++) {
        if (expression.at(i).content == "(") {
            paraCheck += 1;
            if (!(expression.at(i+1).type == TokenType::plus || expression.at(i+1).type == TokenType::minus || expression.at(i+1).type == TokenType::multiply || expression.at(i+1).type == TokenType::divide)) {
                std::cout << "Unexpected token at line " << expression.at(i+1).line << " column " << expression.at(i+1).index << ": " << expression.at(i+1).content << std::endl;
                exit(2);
            }
        }
        if (expression.at(i).content == ")") {
            paraCheck -= 1;
        }

        if (expression.at(i).type == TokenType::plus || expression.at(i).type == TokenType::minus || expression.at(i).type == TokenType::multiply || expression.at(i).type == TokenType::divide) {
            if (expression.at(i - 1).type != TokenType::leftParenthesis) {
                std::cout << "Unexpected token at line " << expression.at(i).line << " column " << expression.at(i).index << ": " << expression.at(i).content << std::endl;
                exit(2);
            }
        }

        if (paraCheck < 0) {          // check para is correct
            std::cout << "Unexpected token at line " << expression.at(i).line << " column " << expression.at(i).index << ": " << expression.at(i).content  << std::endl;
            exit(2);
        }
        if (paraCheck == 0 && i != expression.size() - 2) {
            std::cout << "Unexpected token at line " << expression.at(i+1).line << " column " << expression.at(i+1).index << ": " << expression.at(i+1).content  << std::endl;
            exit(2);
        }
    }
    if (paraCheck != 0) {          // check para is correct
        std::cout << "Unexpected token at line " << expression.at(expression.size() - 1).line << " column " << expression.at(expression.size() - 1).index << ": " << expression.at(expression.size() - 1).content << std::endl;
        exit(2);
    }
}




int main() {
    std::string input = "";
    std::string line;
    
    while(!std::cin.eof()) {
        char c;
        std::cin.get(c);
        line = std::string(1, c);
        input += line;
    }
    input = input.substr(0, input.size()-1);

    
    if (input.size() == 0) 
    {
        std::cout << "Unexpected token at line 1 column 1: END" << std::endl;
        return 2; 
        
    }
    
    std::vector<Token> TokenVector = Token::GenTokenVector(input);
    // Check ERROR
    Token lastToken = TokenVector.back();
    if (lastToken.type == TokenType::error)
    {
        std::cout << "Syntax error on line " << lastToken.line << " column " << lastToken.index << "." << std::endl;
        return 1;
    }

    errorCheck(TokenVector);
    

    

    
    Node root = Parser::MakeTree(TokenVector, 0, TokenVector.size() - 2);
    if (Parser::ErrorToken.type != TokenType::none)
    {
        std::cout << "Unexpected token at line " << Parser::ErrorToken.line << " column " << Parser::ErrorToken.index << ": " << Parser::ErrorToken.content << std::endl;
        return 2;
    }
    Parser::print(root);
    std::cout << std::endl;
    
    double result = Parser::calculate(root);
    if (Parser::ErrorToken.type != TokenType::none)
    {
        std::cout << "Runtime error: division by zero." << std::endl;
        return 3;
    }

    std::cout << result << std::endl;
    
    
    return 0;
}
