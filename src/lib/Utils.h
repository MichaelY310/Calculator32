#pragma once

#include "Lexer.h"

#define DEBUG 0
#define HINT 0

void hint(std::vector<Token> expression, std::vector<std::vector<Token>> expressionLines, int checkIndex, 
            int line=1, int index=7, std::string content="89");

// git add *
// git commit -m "Checkpoint2 partA finished"
// git push origin Michael

// g++ -Wall -Wextra -Werror -std=c++17 parse.cpp -o main lib/Parser.cpp lib/Lexer.cpp