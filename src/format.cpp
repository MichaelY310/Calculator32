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
    std::string input = "(x = 10)\n(y = 57)\n(z = 16)\n((((x = 3) + (y = 5)) + w) + (z = 145))\n((x + y) + z)";
#endif

    if (input.size() == 0) { return 0; }

    std::vector<std::string> expressions;
    std::stringstream ss(input);
    std::string s;

    while (std::getline(ss, s, '\n')) {
        expressions.push_back(s);
    }

    ParserB::parseLines(expressions);
    return 0;
}