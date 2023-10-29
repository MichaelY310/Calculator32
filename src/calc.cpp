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
    //std::string input = "x=7\nx\n(y = ((12 / 4) + 2))\n(a = (b = (c = 12)))\n((a / 4) + (c / 3))";
    //std::string input = "(((a / 4) + (c / 3))";
    //std::string input = "88=mgh\n=\n=x\n=4321\nx=\nx = y =\n88 = mph";
    //std::string input = "x = y = 0 + 1 + 2 * 3 - 4 / (5 + 6)\nb=13\n(7 - (b = (b + 5)))\n1 % 114514\n\n1=a\n1+1\n";
    std::string input = "(x = 10)\n(y = 57)\n(z = 16)\n((((x = 3) + (y = 5)) + w) + (z = 145))\n((x + y) + z)";
    //std::string input = "()+)";
#endif




    if (input.size() == 0) { return 0; }
    std::vector<std::string> expressions;
    std::stringstream ss(input);
    std::string s;

    while (std::getline(ss, s, '\n')) {
        expressions.push_back(s);
    }

    int lineCount = 0;
    for (std::string expression : expressions)
    {

        lineCount += 1;

        if (expression.length() == 0)
        {
            continue;
        }

        // Lexer
        std::vector<Token> TokenVector;
        std::pair<int, int> errorPair = Token::GenTokenVector(expression, TokenVector);
        if (errorPair.first != -1)
        {
            std::cout << "Syntax error on line " << errorPair.first << " column " << errorPair.second << "." << std::endl;
            continue;
        }

        ParserB::setupExpression(TokenVector);

        // ParserB
        std::unique_ptr<ExpressionNode> root;
        std::pair<std::pair<int, int>, std::string> errorResult = ParserB::MakeExpressionTree(TokenVector, 0, TokenVector.size() - 2, root);

        if (errorResult.first.first != -1) 
        {
            std::cout << "Unexpected token at line " << errorResult.first.first << " column " << errorResult.first.second << ": " << errorResult.second << std::endl;
            continue;
        }
        ParserB::print(root.get());
        std::cout << std::endl;

        // Calculate
        double result;
        std::map<std::string, double> originalVariableMap(ParserB::variableMap);
        std::map<std::string, bool> originalVariableInitializedMap(ParserB::variableInitializedMap);
        std::string errorMessage = ParserB::calculate(root.get(), result);

        if (errorMessage.length() != 0)
        {
            std::cout << errorMessage << std::endl;
            ParserB::variableMap.swap(originalVariableMap);
            ParserB::variableInitializedMap.swap(originalVariableInitializedMap);

            continue;
        }
        std::cout << result << std::endl;
    }
    return 0;
}