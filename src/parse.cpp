#include "lib/Parser.h"


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

    if (TokenVector.size() == 1) {
        std::cout << "Unexpected token at line " << TokenVector[0].line << " column " << TokenVector[0].index << ": " << TokenVector[0].content << std::endl;
        return 2;
    }

    Node root = Parser::MakeTree(TokenVector, 0, TokenVector.size() - 2);
    if (Parser::ErrorToken.type != TokenType::none)
    {
        std::cout << "Unexpected token at line " << Parser::ErrorToken.line << " column " << Parser::ErrorToken.index << ": " << Parser::ErrorToken.content << std::endl;
        return 2;
    }
    Parser::print(root);
    std::cout << std::endl;
    
    double result= Parser::calculate(root);
    if (Parser::ErrorToken.type != TokenType::none)
    {
        std::cout << "Runtime error: division by zero." << std::endl;
        return 3;
    }

    std::cout << result << std::endl;
    
    
    return 0;
}
