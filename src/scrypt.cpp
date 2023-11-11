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



//     std::string input = R"(
// def add(a) {
//     if (a <= 1) { 
//         return a;
//     }
//     return a * add(a-1);
// }

// print add(5);
//     )";


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
    print 114514; 
    return;
}
print p();
    )";



 
#endif

    // Lexer
    std::vector<Token> TokenVector;
    std::pair<int, int> errorPair = Token::GenTokenVector(input, TokenVector);
    if (errorPair.first != -1)
    {
        std::cout << "Syntax error on line " << errorPair.first << " column " << errorPair.second << "." << std::endl;
        exit(1);
    }
    // Token::printLexer(TokenVector);

    // Parser
    // Add the global Scope
    ParserB::ScopeStack.push(Scope());
    ParserB::setupExpression(TokenVector);
    std::vector<std::unique_ptr<Node>> flows;
    std::pair<std::pair<int, int>, std::string> errorResult = ParserB::HandleTokenVector(TokenVector, 0, TokenVector.size()-2, flows);
    if (errorResult.first.first != -1) 
    {
        std::cout << "Unexpected token at line " << errorResult.first.first << " column " << errorResult.first.second << ": " << errorResult.second << std::endl;
        exit(2);
    }

    // for (int i=0; i < (int)flows.size(); i++)
    // {
    //     ParserB::print(flows[i].get());
    //     std::cout << std::endl;
    // }

    // Calculate
    // std::cout << "start calculating" << std::endl;
    for (int i=0; i < (int)flows.size(); i++)
    {
        Result result;
        std::string errorMessage = ParserB::calculate(flows[i].get(), result);

        if (errorMessage.length() != 0)
        {
            std::cout << errorMessage << std::endl;
            exit(3);
        }
    }
    return 0;
}
