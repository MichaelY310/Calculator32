#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

enum class TokenType {
    number,
    plus,
    minus,
    multiply,
    divide,
    leftParenthesis,
    rightParenthesis,
    end,
    error
};

class Token {

public:
    Token(TokenType itype, std::string icontent, int iline, int iindex, double ivalue = -1);
    ~Token() = default;

    operator double() const {
        return value;
    }

public:
    static std::vector<Token> GenTokenVector(const std::string input)
    {
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
            // std::cout << (int)input.at(i) << std::endl;
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
                // nothing before a point
                if (!recordingNumber)
                {
                    // if there's error, the last token would be ERROR instead of END
                    res.emplace_back(TokenType::error, "ERROR", line, index, -1);
                    return res;
                }
                // already have a point
                if (afterPoint)
                {
                    // if there's error, the last token would be ERROR instead of END
                    res.emplace_back(TokenType::error, "ERROR", line, index, -1);
                    return res;
                }
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
                    res.emplace_back(TokenType::error, "ERROR", line, index, -1);
                    return res;
                }
            }
            index++;
        }

        res.emplace_back(TokenType::end, "END", line, index, -1);

        return res;
    }

    static void printLexer(std::vector<Token> TokenVector);
    static void printLexer(const std::string input);
     
public:
    TokenType type;
    std::string content;    // value as a string
    int line = -1;
    int index = -1;
    double value = -1;       // a number has its own value. otherwise -1

};

