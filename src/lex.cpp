
#include "lib/Lexer.h"
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
    //std::string input = "(= foo_ b 3)\n( + b 0 )\n(- (= b (+ b 5)) 7)\n(* foo b)\n";
    //std::string input = "a\n";    
//     std::string input = R"((x = 42)
// (steps = 0)
// while (x > 1) {
//     (steps = (steps + 1))
//     if ((x % 2) == 0) {
//         (x = (x / 2))
//     }
//     else {
//         (x = ((3 * x) + 1))
//     }
// }
// print steps)";    
    //std::string input = "(=(n) 9)";
    std::string input = "[9]";
#endif

    std::vector<Token> TokenVector;
    std::pair<int, int> errorPair = Token::GenTokenVector(input, TokenVector);

    int errorLine = errorPair.first;
    int errorIndex = errorPair.second;
    if (errorLine != -1)
    {
        std::cout << "Syntax error on line " << errorLine << " column " << errorIndex << "." << std::endl;
        exit(1);
    }
    Token::printLexer(TokenVector);
    return 0;
}