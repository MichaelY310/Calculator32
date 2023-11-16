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
#endif
#if DEBUG == 1
    std::string input = R"(
(z = 42);
def foo(x, y) {
    def square(value) {
        return (value * value);
    }
    print square(((x + y) + z));
}
(z = 108);
(f = foo);
(result = f(1, 2));
if (result != null) {
    print result;
}

def p() { 
    print a; 
    return;
}
p();
def p() {
    return null;
}
    )";

#endif
    // std::cout << "===================" << std::endl;
    // std::cout << input << std::endl;
    // std::cout << "===================" << std::endl;
    std::vector<Token> TokenVector;
    std::pair<int, int> errorPair = Token::GenTokenVector(input, TokenVector);

    if (errorPair.first != -1)
    {
        std::cout << "Syntax error on line " << errorPair.first << " column " << errorPair.second << "." << std::endl;
        exit(1);
    }
    // Token::printLexer(TokenVector);
    std::vector<std::unique_ptr<Node>> flows;
    std::pair<std::pair<int, int>, std::string> errorResult = ParserB::HandleTokenVector(TokenVector, 0, TokenVector.size()-2, flows);
    if (errorResult.first.first != -1) 
    {
        std::cout << "Unexpected token at line " << errorResult.first.first << " column " << errorResult.first.second << ": " << errorResult.second << std::endl;
        exit(2);
    }
    // std::cout << "runn" << std::endl;
    for (int i=0; i < (int)flows.size(); i++)
    {
        ParserB::print(flows[i].get());
        // if (flows[i].get()->value.type == TokenType::ARRAY) {
        //     ArrayNode* Array = dynamic_cast<ArrayNode*>(flows[i].get());
        //     if (Array->ArrayContent[2]->value.type == TokenType::NUMBER) {
        //         std::cout << "yes" << std::endl;
        //     }
        //     std::cout << Array->ArrayContent[2]->value << std::endl;
        // }
        // if (flows[i].get()->value.type == TokenType::VARIABLE) {
        //     std::cout << flows[i].get()->value.content << std::endl;
        // }


        std::cout << std::endl;
    }
    return 0; 
}