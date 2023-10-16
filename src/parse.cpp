#include "lib/parse.h"


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
    
    //std::string input = "(* (+ 1 2) 3 (/ 4 5 (- 6 7)))";
    //std::string input = "(/ 1 0)";
    //std::string input = "7";
    //std::string input = "(1 * 2 * 3)";

    
    if (input.size() == 0) { return 2; }
    if (input.at(0) != '(') 
    {
        std::cout << "Unexpected token at line " << Parser::ErrorToken.line << "  column  " << Parser::ErrorToken.index << ": " << Parser::ErrorToken.content << std::endl;

        return 2; 
        
    }
        if (input.at(input.size()-1) != ')') 
    {
        std::cout << "Unexpected token at line " << Parser::ErrorToken.line << "  column  " << Parser::ErrorToken.index << ": " << Parser::ErrorToken.content << std::endl;

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

    
    Node root = Parser::MakeTree(TokenVector, 0, TokenVector.size() - 2);
    if (Parser::ErrorToken.type != TokenType::none)
    {
        std::cout << "Unexpected token at line " << Parser::ErrorToken.line << "  column  " << Parser::ErrorToken.index << ": " << Parser::ErrorToken.content << std::endl;
        return 2;
    }
    // Parser::print(root);
    
    double result= Parser::calculate(root);
    if (Parser::ErrorToken.type != TokenType::none)
    {
        std::cout << "Runtime error: division by zero." << std::endl;
        return 3;
    }

    Parser::print(root);
    std::cout << std::endl;
    std::cout << result << std::endl;
    
    
    return 0;
}
