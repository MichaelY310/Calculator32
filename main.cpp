#include <string>
#include <iostream>
#include "src/lib/lex.h"

int main() {
    std::cout << "hello world" << std::endl;
    // Token::printLexer("(+(-2 4.444 )\n32(* 5 13.45)(");
    Token::printLexer("(/49 50.\n12 ($ 7..8 .3))");
    return 0;

}