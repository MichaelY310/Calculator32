#include "lib/Parser.h"
#include "lib/Lexer.h"

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

    Parser p(TokenVector);
    p.printinfix();
    double result;

    result = p.evaluateExpression();
    std::cout << result << std::endl;


    return 0;

}
   