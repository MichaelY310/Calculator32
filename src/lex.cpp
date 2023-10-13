#include "lib/lex.h"


Token::Token(TokenType itype, std::string icontent, int iline, int iindex, double ivalue)
    : type(itype), content(icontent), line(iline), index(iindex), value(ivalue)
{

}

void Token::printLexer(std::vector<Token> TokenVector) 
{


    // Check ERROR
    Token lastToken = TokenVector.back();
    if (lastToken.type == TokenType::error)
    {
        std::cout << "Syntax error on line " << lastToken.line << " column " << lastToken.index << "." << std::endl;
        return;
    }


    int spaceNumber;
    std::string content; // line or index to string
    for (Token t : TokenVector)
    {
        // line
        content = std::to_string(t.line);
        spaceNumber = 4 - content.length();
        for (int i=0; i<spaceNumber; i++)
        {
            std::cout << " ";
        }
        std::cout << content;

        // index
        content = std::to_string(t.index);
        spaceNumber = 5 - content.length();
        for (int i=0; i<spaceNumber; i++)
        {
            std::cout << " ";
        }
        std::cout << content;

        std::cout << " " << " ";

        // token value
        std::cout << t.content << std::endl;;

        
        // std::cout << t.line << " " << t.index << " " << t.value << " " << t.content << std::endl;
    }

    return;
}



void Token::printLexer(const std::string input) 
{
    std::vector<Token> TokenVector = GenTokenVector(input);
    // Check ERROR
    Token lastToken = TokenVector.back();
    if (lastToken.type == TokenType::error)
    {
        std::cout << "Syntax error on line " << lastToken.line << " column " << lastToken.index << "." << std::endl;
        return;
    }
    int spaceNumber;
    std::string content; // line or index to string
    for (Token t : TokenVector)
    {
        // line
        content = std::to_string(t.line);
        spaceNumber = 4 - content.length();
        for (int i=0; i<spaceNumber; i++)
        {
            std::cout << " ";
        }
        std::cout << content;
        // index
        content = std::to_string(t.index);
        spaceNumber = 5 - content.length();
        for (int i=0; i<spaceNumber; i++)
        {
            std::cout << " ";
        }
        std::cout << content;
        std::cout << " " << " ";
        // token value
        std::cout << t.content << std::endl;;
        
        // std::cout << t.line << " " << t.index << " " << t.value << " " << t.content << std::endl;
    }
    return;
}


int main() {
    // std::cout << "hello world" << std::endl;
    // // Token::printLexer("(+(-2 4.444 )\n32(* 5 13.45)(");
    // Token::printLexer("(+(-2 4.444 )");
    // // Token::printLexer("(/49 50.\n12 ($ 7..8 .3))");
    // return 0;


    // read input
    std::string input;
    while (true) {
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            // Stop reading if the line is empty
            break;
        }

        // Concatenate the lines into the input string
        input += line + '\n';
    }

    // input = "(+(-2 4.444 )";

    std::vector<Token> TokenVector = Token::GenTokenVector(input);
    // Check ERROR
    Token lastToken = TokenVector.back();
    if (lastToken.type == TokenType::error)
    {
        std::cout << "Syntax error on line " << lastToken.line << " column " << lastToken.index << "." << std::endl;
        return 1;
    }


    Token::printLexer(TokenVector);
    return 0;
}