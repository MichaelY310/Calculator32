#include <string>
#include <iostream
#include "src/lib/lex.h"
#include "src/lib/parse.h"

int main() {
    std::string input = "(+3.1 (+4 4)  44 4)";
    std::vector<Token> tokens = Token::GenTokenVector(input);
    Parser parser(tokens);
    double result = parser.parseExpression();
    std::string parserString = parser.getStringExpression();
    std::cout << parserString << std::endl;
    std::cout << "Result: " << result << std::endl;
    return 0;
}