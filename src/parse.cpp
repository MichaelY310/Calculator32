#include "lib/Parser.h"

void errorCheck(std::vector<Token> expression) {
    if (expression.size() == 1 || !(expression.at(0).type == TokenType::leftParenthesis || expression.at(0).type == TokenType::number)) {
        // std::cout << "8" << std::endl;
        std::cout << "Unexpected token at line " << expression.at(0).line << " column " << expression.at(0).index << ": " << expression.at(0).content << std::endl;
        exit(2);
    }
    if (expression.at(0).type == TokenType::number) {
        if (expression.size() != 2) {
            // std::cout << "9" << std::endl;
            std::cout << "Unexpected token at line " << expression.at(1).line << " column " << expression.at(1).index << ": " << expression.at(1).content << std::endl;
            exit(2);
        }
        else {
            return;
        }
    }

    if (!(expression.at(1).type == TokenType::plus || expression.at(1).type == TokenType::minus || expression.at(1).type == TokenType::multiply || expression.at(1).type == TokenType::divide || expression.at(1).type == TokenType::equals)) {
        // std::cout << "10" << std::endl;
        std::cout << "Unexpected token at line " << expression.at(1).line << " column " << expression.at(1).index << ": " << expression.at(1).content << std::endl;
        exit(2);
    }

    int paraCheck = 1;
    for (size_t i = 1; i < expression.size() - 1; i++) {
        if (expression.at(i).content == "(") {
            paraCheck += 1;
            if (!(expression.at(i+1).type == TokenType::plus || expression.at(i+1).type == TokenType::minus || expression.at(i+1).type == TokenType::multiply || expression.at(i+1).type == TokenType::divide || expression.at(i+1).type == TokenType::equals)) {
                // std::cout << "11" << std::endl;
                std::cout << "Unexpected token at line " << expression.at(i+1).line << " column " << expression.at(i+1).index << ": " << expression.at(i+1).content << std::endl;
                exit(2);
            }
        }
        if (expression.at(i).content == ")") {
            paraCheck -= 1;
        }

        if (expression.at(i).type == TokenType::plus || expression.at(i).type == TokenType::minus || expression.at(i).type == TokenType::multiply || expression.at(i).type == TokenType::divide || expression.at(i).type == TokenType::equals) {
            if (expression.at(i - 1).type != TokenType::leftParenthesis) {
                // std::cout << "12" << std::endl;
                std::cout << "Unexpected token at line " << expression.at(i).line << " column " << expression.at(i).index << ": " << expression.at(i).content << std::endl;
                exit(2);
            }
        }

        if (paraCheck < 0) {          // check para is correct
            // std::cout << "13" << std::endl;
            std::cout << "Unexpected token at line " << expression.at(i).line << " column " << expression.at(i).index << ": " << expression.at(i).content  << std::endl;
            exit(2);
        }
        if (paraCheck == 0 && i != expression.size() - 2) {
            // std::cout << "14" << std::endl;
            std::cout << "Unexpected token at line " << expression.at(i+1).line << " column " << expression.at(i+1).index << ": " << expression.at(i+1).content  << std::endl;
            exit(2);
        }
    }
    if (paraCheck != 0) {          // check para is correct
        // std::cout << "15" << std::endl;
        std::cout << "Unexpected token at line " << expression.at(expression.size() - 1).line << " column " << expression.at(expression.size() - 1).index << ": " << expression.at(expression.size() - 1).content << std::endl;
        exit(2);
    }
}

int main() {
    // std::string input = "";
    // std::string line;
    
    // while(!std::cin.eof()) {
    //     char c;
    //     std::cin.get(c);
    //     line = std::string(1, c);
    //     input += line;
    // }
    // input = input.substr(0, input.size()-1);

    
    //std::string input = "(* (+ 1 2) 3 (/ 4 5 (- 6 7)))\n(* (+ 1 2) 3 (/ 4 5 (- 6 7)))\n(* (+ 1 2) 3 (/ 4 5 (- 6 7)))";
    //std::string input = "(= a b 3)";
    //std::string input = "(- (= b (+ b 5)) 7)";
    //std::string input = "(* a b)";
    std::string input = "(= a b 3)\n( + b 0 )\n(- (= b (+ b 5)) 7)\n(* a b)";


    if (input.size() == 0) 
    {
        std::cout << "Unexpected token at line 1 column 1: END" << std::endl;
        return 2; 
    }
    
    std::vector<Token> TokenVector = Token::GenTokenVector(input);


    Parser::setupExpression(TokenVector);
    //std::cout << Parser::expressionLines.size() << std::endl;
    for (std::vector<Token> expressionLine : Parser::expressionLines)
    {
        //Token::printLexer(expressionLine);
        errorCheck(expressionLine);
        Node root = Parser::MakeTree(expressionLine, 0, expressionLine.size() - 2);
        Parser::print(root);
        std::cout << std::endl;
        std::cout << Parser::calculate(root) << std::endl;
    }
    
    return 0;
}
