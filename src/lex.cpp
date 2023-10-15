#include "lib/Lexer.h"

int main() {
    std::string input = "";
    std::string line;

    while (!std::cin.eof()) {
        char c;
        std::cin.get(c);
        line = std::string(1, c);
        input += line;
    }
    input = input.substr(0, input.size() - 1);

    std::vector<Token> TokenVector = Token::GenTokenVector(input);
    // Check ERROR
    Token lastToken = TokenVector.back();
    if (lastToken.type == TokenType::error)
    {
        std::cout << "Syntax error on line " << lastToken.line << " column " << lastToken.index << "." << std::endl;
        return 1;
    }


    Token::printLexer(TokenVector);
    return 0;
}