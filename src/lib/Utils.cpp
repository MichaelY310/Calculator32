#include "Utils.h"

void hint(std::vector<Token> expression, std::vector<std::vector<Token>> expressionLines, int checkIndex, 
            int line=1, int index=7, std::string content="89")
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
