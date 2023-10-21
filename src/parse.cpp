#include "lib/Parser.h"

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
    
    //std::string input = "(* (+ 1 2) 3 (/ 4 5 (- 6 7)))\n(* (+ 1 2) 3 (/ 4 5 (- 6 7)))\n(* (+ 1 2) 3 (/ 4 5 (- 6 7)))";
    //std::string input = "(= a b 3)";
    //std::string input = "(- (= b (+ b 5)) 7)";
    //std::string input = "(* a b)";
    // std::string input = "9\n(= foo b 3)\n( + b 0 )\n(- (= b (+ b 5)) 7)\n(* foo b)\n";
    // std::string input = "\t \n  (   - 3  -)";
    std::string input = "( = a ( + 1 2 ) 3 )";
    std::string input = "( = a ( + 2 2 ) 4 )";
    //std::string input = "12\n";
    //std::string input = "  \n          (- 1 10)\n";    
    // std::string input = "(=(n) 9)";
#endif

    if (input.size() == 0) { std::cout << "Unexpected token at line 1 column 1: END" << std::endl; return 2;  }
    
    std::vector<Token> TokenVector = Token::GenTokenVector(input);

    Parser::setupExpression(TokenVector);
    //std::cout << Parser::expressionLines.size() << std::endl;
    for (std::vector<Token> expressionLine : Parser::expressionLines)
    {
        // Token::printLexer(expressionLine);
        // std::cout << "==================" << std::endl;

        Node root = Parser::MakeTree(expressionLine, 0, expressionLine.size() - 2);
        Parser::print(root);
        std::cout << std::endl;
        std::cout << Parser::calculate(root) << std::endl;
    }
    
    return 0;
}
