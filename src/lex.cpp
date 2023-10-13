#include "lib/Lexer.h"

int main() {
    // std::cout << "hello world" << std::endl;
    // // Token::printLexer("(+(-2 4.444 )\n32(* 5 13.45)(");
    // Token::printLexer("(+(-2 4.444 )");
    // // Token::printLexer("(/49 50.\n12 ($ 7..8 .3))");
    // return 0;


    // read input
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

    // input = "(+(-2 4.444 )";

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