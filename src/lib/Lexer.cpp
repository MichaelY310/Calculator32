#include "Lexer.h"

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

    // for variable
    int variableLength = 0;
    std::string variableName;
    bool recordingVariable = false;

    for (int i = 0; i < len; i++)
    {
        // std::cout << (int)input.at(i) << std::endl;

        // variable
        if ((input.at(i) >= 'a' && input.at(i) <= 'z') || (input.at(i) >= 'A' && input.at(i) <= 'Z') || input.at(i) == '_')
        {
            // 123abc
            if (recordingNumber)
            {
#if DEBUG
    std::cout << "1" << std::endl;
#endif
                std::cout << "Syntax error on line " << line << " column " << i << "." << std::endl;
                exit(1);
            }
            else 
            {
                recordingVariable = true;
                std::string s(1, input.at(i));
                variableName += s;
                variableLength += 1;
            }
        }

        // number
        else if (input.at(i) == '0' || input.at(i) == '1' || input.at(i) == '2' || input.at(i) == '3' || input.at(i) == '4' || input.at(i) == '5' || input.at(i) == '6' || input.at(i) == '7' || input.at(i) == '8' || input.at(i) == '9')
        {
            // recording variable
            // ab123
            if (recordingVariable)
            {
                std::string s(1, input.at(i));
                variableName += s;
                variableLength += 1;
            }
            else 
            {
                recordingNumber = true;
                if (afterPoint) {
                    currentValue += double(input.at(i) - '0') / std::pow(10.0f, afterPoint);
                    afterPoint += 1;
                }
                else {
                    currentValue *= 10;
                    currentValue += double(input.at(i) - '0');
                }
                std::string s(1, input.at(i));
                currentStringValue += s;
                numberLength += 1;
            }
        }

        // .
        else if (input.at(i) == '.')
        {
            // nothing before a point
            if (!recordingNumber)
            {
                // if there's error, the last token would be ERROR instead of END
#if DEBUG
    std::cout << "2" << std::endl;
#endif

                std::cout << "Syntax error on line " << line << " column " << index << "." << std::endl;
                exit(1);
            }
            // already have a point
            if (afterPoint)
            {
                // if there's error, the last token would be ERROR instead of END
#if DEBUG
    std::cout << "3" << std::endl;
#endif

                std::cout << "Syntax error on line " << line << " column " << index << "." << std::endl;
                exit(1);
            }
            afterPoint += 1;
            numberLength += 1;
            recordingNumber = true;
            currentStringValue += ".";
        }

        // operations, space, \t, \n
        else {
            // ERROR if point not followed by a number
            if (recordingNumber && currentStringValue.back() == '.')
            {
                // if there's error, the last token would be ERROR instead of END
#if DEBUG
    std::cout << "4" << std::endl;
#endif

                std::cout << "Syntax error on line " << line << " column " << index << "." << std::endl;
                exit(1);
            }


            // number ends
            // the last digit is recorded
            if (recordingNumber) {
                res.emplace_back(TokenType::number, currentStringValue, line, index - numberLength, currentValue);
                recordingNumber = false;
                afterPoint = false;
                currentValue = 0;
                currentStringValue = "";
                numberLength = 0;
            }

            // variable ends
            // the last character of variable is recorded
            if (recordingVariable) {
                res.emplace_back(TokenType::variable, variableName, line, index - variableLength, -1);
                recordingVariable = false;
                variableName = "";
                variableLength = 0;
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
            else if (input.at(i) == '=')
            {
                res.emplace_back(TokenType::equals, "=", line, index, -1);
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
#if DEBUG
    std::cout << "5" << std::endl;
#endif

                std::cout << "Syntax error on line " << line << " column " << index << "." << std::endl;
                exit(1);
            }
        }
        index++;
    }

    // final checks
    if (recordingNumber && currentStringValue.back() == '.')
    {
        // if there's error, the last token would be ERROR instead of END
#if DEBUG
    std::cout << "6" << std::endl;
#endif

        std::cout << "Syntax error on line " << line << " column " << index << "." << std::endl;
        exit(1);
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

    // variable ends
    // the last character of variable is recorded
    if (recordingVariable) {
        res.emplace_back(TokenType::variable, variableName, line, index - variableLength, -1);
        recordingVariable = false;
        variableName = "";
        variableLength = 0;
    }


    res.emplace_back(TokenType::end, "END", line, index, -1);

    return res;
}


void Token::printLexer(std::vector<Token> TokenVector)
{
    int spaceNumber;
    std::string content; // line or index to string
    for (Token t : TokenVector)
    {
        // line
        content = std::to_string(t.line);
        spaceNumber = 4 - content.length();
        for (int i = 0; i < spaceNumber; i++)
        {
            std::cout << " ";
        }
        std::cout << content;

        // index
        content = std::to_string(t.index);
        spaceNumber = 5 - content.length();
        for (int i = 0; i < spaceNumber; i++)
        {
            std::cout << " ";
        }
        std::cout << content;

        std::cout << " " << " ";

        // token value
        std::cout << t.content << std::endl;
    }

    return;
}



void Token::printLexer(const std::string input)
{
    std::vector<Token> TokenVector = GenTokenVector(input);
    Token::printLexer(TokenVector);
}
