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
def p() {}
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

    for (int i=0; i < (int)flows.size(); i++)
    {
        ParserB::print(flows[i].get());
        std::cout << std::endl;
    }
    return 0;
}