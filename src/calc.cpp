#include "lib/ParserB.h"

int main() {
#if DEBUG == 0
    std::string input = "";
    std::string line;
    
    while(!std::cin.eof()) {
        char c;
        std::cin.get(c);
        line = std::string(1, c);
        input += line;
    }
    input = input.substr(0, input.size()-1);
    // std::cout << "LOLOLOLOLOLOLOLOLO" << std::endl;
    // std::cout << input << std::endl;
    // std::cout << "LOLOLOLOLOLOLOLOLO" << std::endl;
#endif
#if DEBUG == 1
    
    //std::string input = "x = y = 0 + 1 + 2 * 3 - 4 / (5 + 6)";
    //std::string input = "b=13\n(7 - (b = (b + 5)))";
    std::string input = "(7 - 5))";
#endif

    if (input.size() == 0) { std::cout << "Unexpected token at line 1 column 1: END" << std::endl; return 2;  }
    
    std::vector<Token> TokenVector = Token::GenTokenVector(input);

    ParserB::setupExpressionInfix(TokenVector);
    //std::cout << ParserB::expressionLines.size() << std::endl;
    for (std::vector<Token> expressionLine : ParserB::expressionLines)
    {
        // Token::printLexer(expressionLine);
        // std::cout << "==================" << std::endl;

        Node root = ParserB::MakeTreeInfix(expressionLine, 0, expressionLine.size() - 2);
        ParserB::print(root);
        std::cout << std::endl;
        std::cout << ParserB::calculate(root) << std::endl;

    }
    
    return 0;
}