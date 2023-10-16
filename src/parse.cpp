#include "lib/Parser.h"
#include "lib/Lexer.h"

int main() {
    std::string input = "";
    std::string line;

    // while (!std::cin.eof()) {
    //     char c;
    //     std::cin.get(c);
    //     line = std::string(1, c);
    //     input += line;
    // }
    // input = input.substr(0, input.size() - 1);
    input = "(* (+ 1 2) 3 (/ 4 5 (- 6 7)))";

    std::vector<Token> TokenVector = Token::GenTokenVector(input);

    Parser p(TokenVector);
    p.printinfix();
    double result;

    result = p.evaluateExpression();
    std::cout << result << std::endl;

    return 0;

}
   