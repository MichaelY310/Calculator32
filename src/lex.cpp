#include "lib/lex.h"


Token::Token(TokenType itype, std::string icontent, int iline, int iindex, double ivalue)
    : type(itype), content(icontent), line(iline), index(iindex), value(ivalue)
{

}



std::vector<Token> Token::GenTokenVector(const std::string input) {
    int len = input.length();

    std::vector<Token> res;

    int line = 1;
    int index = 1;

    // for number
    int numberLength = 0;
    double currentValue = 0;         
    std::string currentStringValue = "";
    double afterPoint = 0;   
    bool recordingNumber = false;

    for (int i = 0; i < len; i++)
    {
        if (input.at(i) == '0' || input.at(i) == '1' || input.at(i) == '2' || input.at(i) == '3' || input.at(i) == '4' || input.at(i) == '5' || input.at(i) == '6' || input.at(i) == '7' || input.at(i) == '8' || input.at(i) == '9') 
        {
            recordingNumber = true;
            if (afterPoint) {
                currentValue += double(input.at(i) - '0') / std::pow(10.0f, afterPoint);
                afterPoint += 1;
            } else {
                currentValue *= 10;
                currentValue += double(input.at(i) - '0');   
            }
            std::string s(1, input.at(i));
            currentStringValue += s;
            numberLength += 1;
        }
        else if (input.at(i) == '.')
        {
            afterPoint += 1;
            numberLength += 1;
            recordingNumber = true;
            currentStringValue += ".";
        }
        else {
            // ERROR if point not followed by a number
            if (recordingNumber && currentStringValue.back() == '.')
            {
                // if there's error, the last token would be ERROR instead of END
                res.emplace_back(TokenType::error, "ERROR", line, index, -1);
                return res;
            }


            // the last digit is recorded
            if (recordingNumber) {
                res.emplace_back(TokenType::number, currentStringValue, line, index - numberLength, currentValue);
                recordingNumber = false;
                afterPoint = false;
                currentValue = 0;
                currentStringValue = "";
                numberLength = 0;
            }

            if (input.at(i) == '\n')
            {
                line++;
                index = 0;
            }
            else if (input.at(i) == ' ')
            {
            }
            else if (input.at(i) == '\t')
            {
            }
            else if (input.at(i) == '+')
            {
                res.emplace_back(TokenType::plus, "+", line, index, -1);
            }
            else if (input.at(i) == '-')
            {
                res.emplace_back(TokenType::minus, "-", line, index, -1);
            }
            else if (input.at(i) == '*')
            {
                res.emplace_back(TokenType::multiply, "*", line, index, -1);
            }
            else if (input.at(i) == '/')
            {
                res.emplace_back(TokenType::divide, "/", line, index, -1);
            }
            else if (input.at(i) == '(')
            {
                res.emplace_back(TokenType::leftParenthesis, "(", line, index, -1);
            }
            else if (input.at(i) == ')')
            {
                res.emplace_back(TokenType::rightParenthesis, ")", line, index, -1);
            }
            // Error
            else    
            {
                // if there's error, the last token would be ERROR instead of END
                res.emplace_back(TokenType::error, "ERROR", line+1, index, -1);
                return res;
            }
        }
        index++;
    }

    res.emplace_back(TokenType::end, "END", line+1, 1, -1);

    return res;
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



// int main() {

//     std::string input;
//     std::cin >> input;
//     std::vector<Token> TokenVector = Token::GenTokenVector(input);

//     std::cout << TokenVector.size() << std::endl;

//     Token::printLexer(TokenVector);

//     return 1;
// }

int main() {
    // std::cout << "hello world" << std::endl;
    // // Token::printLexer("(+(-2 4.444 )\n32(* 5 13.45)(");
    // Token::printLexer("(+(-2 4.444 )");
    // // Token::printLexer("(/49 50.\n12 ($ 7..8 .3))");
    // return 0;



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


    std::vector<Token> TokenVector = GenTokenVector(input);
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