#include "Utils.h"


void hint(std::vector<Token> expression, std::vector<std::vector<Token>> expressionLines, int checkIndex, 
            int line, int index, std::string content)
{
    if (expression[checkIndex].line == line && expression[checkIndex].index == index && expression[checkIndex].content == content)
    {
        for (std::vector<Token> v : expressionLines)
        {
            for (Token token : v)
            {
                std::cout << token.content << " ";
            }
            std::cout << std::endl;
        }
    }
}
