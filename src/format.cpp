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
//     std::string input = R"(x     = 42
// steps = 0

// while x > 1 {
//   steps = steps + 1
//   if x % 2 == 0 {
//     x = x / 2
//   }
//   else {
//     x = 3 * x + 1
//   }
// }

// print steps)";  

    std::string input = R"(print (a = 49)
print (b = 21)
while (a != b) {
    if (a > b) {
        (a = (a - b))
    }
    else {
        if (b > a) {
            (b = (b - a))
        }
    }
}
print a)";

#endif

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