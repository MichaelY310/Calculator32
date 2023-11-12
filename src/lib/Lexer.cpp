#include "Lexer.h"

Token::Token(TokenType itype, std::string icontent, int iline, int iindex, double ivalue)
    : type(itype), content(icontent), line(iline), index(iindex), value(ivalue)
{
}

std::pair<int, int> Token::GenTokenVector(const std::string& input, std::vector<Token>& res) {
    int len = input.length();

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
        // variable
        if ((input.at(i) >= 'a' && input.at(i) <= 'z') || (input.at(i) >= 'A' && input.at(i) <= 'Z') || input.at(i) == '_')
        {
            // 123abc
            if (recordingNumber)
            {
#if DEBUG
    std::cout << "1" << std::endl;
#endif
                return { line, i };
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
#if DEBUG
    std::cout << "2" << std::endl;
#endif
                return { line, index };
            }
            // already have a point
            if (afterPoint)
            {
#if DEBUG
    std::cout << "3" << std::endl;
#endif

                return { line, index };
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
#if DEBUG
    std::cout << "4" << std::endl;
#endif
                return { line, index };
            }

            // number ends
            // the last digit is recorded
            if (recordingNumber) {
                res.emplace_back(TokenType::NUMBER, currentStringValue, line, index - numberLength, currentValue);
                recordingNumber = false;
                afterPoint = false;
                currentValue = 0;
                currentStringValue = "";
                numberLength = 0;
            }

            // variable ends
            // the last character of variable is recorded
            if (recordingVariable) {
                if (variableName == "while") { res.emplace_back(TokenType::WHILE, variableName, line, index - variableLength, -1); }
                else if (variableName == "if") { res.emplace_back(TokenType::IF, variableName, line, index - variableLength, -1); }
                else if (variableName == "else") { res.emplace_back(TokenType::ELSE, variableName, line, index - variableLength, -1); }
                else if (variableName == "true") { res.emplace_back(TokenType::TRUE, variableName, line, index - variableLength, 1); }
                else if (variableName == "false") { res.emplace_back(TokenType::FALSE, variableName, line, index - variableLength, 0); }
                else if (variableName == "print") { res.emplace_back(TokenType::PRINT, variableName, line, index - variableLength, -1); }
                else if (variableName == "def") { res.emplace_back(TokenType::DEF, variableName, line, index - variableLength, -1); }
                else if (variableName == "null") { res.emplace_back(TokenType::NUL, variableName, line, index - variableLength, -1); }
                else if (variableName == "return") { res.emplace_back(TokenType::RETURN, variableName, line, index - variableLength, -1); }
                else { res.emplace_back(TokenType::VARIABLE, variableName, line, index - variableLength, -1); }
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
                if ( i + 1 < len && input.at(i+1) == '='){
                    res.emplace_back(TokenType::EQUALITY,"==", line, index, -1);
                    i++;
                    index++;
                }
                else{
                    res.emplace_back(TokenType::ASSIGNMENT, "=", line, index, -1);
                }
            }
            else if (input.at(i) == '>'){
                if ( i + 1 < len && input.at(i+1) == '='){
                    res.emplace_back(TokenType::BIGGER_EQUAL,">=", line, index, -1);
                    i++;
                    index++;
                }
                else{
                    res.emplace_back(TokenType::BIGGER, ">", line, index, -1);
                }
            }
            else if (input.at(i) == '<'){
                if ( i + 1 < len && input.at(i+1) == '='){
                    res.emplace_back(TokenType::SMALLER_EQUAL,"<=", line, index, -1);
                    i++;
                    index++;
                }
                else{
                    res.emplace_back(TokenType::SMALLER, "<", line, index, -1);
                }
            }
            else if (input.at(i) == '!'){
                if ( i + 1 < len && input.at(i+1) == '='){
                    res.emplace_back(TokenType::INEQUALITY,"!=", line, index, -1);
                    i++;
                    index++;
                }
                else{
                    return  {line, index};
                }
            }
            else if (input.at(i) == '&')
            {
                res.emplace_back(TokenType::AND, "&", line, index, -1);
            }
            else if (input.at(i) == '|')
            {
                res.emplace_back(TokenType::INCLUSIVE_OR, "|", line, index, -1);
            }
            else if (input.at(i) == '^')
            {
                res.emplace_back(TokenType::EXCLUSIVE_OR, "^", line, index, -1);
            }
            else if (input.at(i) == '+')
            {
                res.emplace_back(TokenType::PLUS, "+", line, index, -1);
            }
            else if (input.at(i) == '-')
            {
                res.emplace_back(TokenType::MINUS, "-", line, index, -1);
            }
            else if (input.at(i) == '*')
            {
                res.emplace_back(TokenType::MULTIPLY, "*", line, index, -1);
            }
            else if (input.at(i) == '/')
            {
                res.emplace_back(TokenType::DIVIDE, "/", line, index, -1);
            }
            else if (input.at(i) == '%')
            {
                res.emplace_back(TokenType::MOD, "%", line, index, -1);
            }
            else if (input.at(i) == '(')
            {
                res.emplace_back(TokenType::LEFT_PARENTHESIS, "(", line, index, -1);
            }
            else if (input.at(i) == ')')
            {
                res.emplace_back(TokenType::RIGHT_PARENTHESIS, ")", line, index, -1);
            }
            else if (input.at(i) == '[')
            {
                res.emplace_back(TokenType::LEFT_BRACKET, "[", line, index, -1);
            }
            else if (input.at(i) == ']')
            {
                res.emplace_back(TokenType::RIGHT_BRACKET, "]", line, index, -1);
            }
            else if (input.at(i) == '{')
            {
                res.emplace_back(TokenType::LEFT_BRACE, "{", line, index, -1);
            }
            else if (input.at(i) == '}')
            {
                res.emplace_back(TokenType::RIGHT_BRACE, "}", line, index, -1);
            }
            else if (input.at(i) == '[')
            {
                res.emplace_back(TokenType::LEFT_BRACKET, "[", line, index, -1);
            }
            else if (input.at(i) == ']')
            {
                res.emplace_back(TokenType::RIGHT_BRACKET, "]", line, index, -1);
            }
            else if (input.at(i) == ',')
            {
                res.emplace_back(TokenType::COMMA, ",", line, index, -1);
            }
            else if (input.at(i) == ';')
            {
                res.emplace_back(TokenType::SEMICOLON, ";", line, index, -1);
            }
            // Error
            else
            {
#if DEBUG
    std::cout << "5" << std::endl;
#endif
                return { line, index };
            }
        }
        index++;
    }

    // final checks
    if (recordingNumber && currentStringValue.back() == '.')
    {
#if DEBUG
    std::cout << "6" << std::endl;
#endif
        return { line, index };
    }

    // the last digit is recorded
    if (recordingNumber) {
        res.emplace_back(TokenType::NUMBER, currentStringValue, line, index - numberLength, currentValue);
    }

    // variable ends
    // the last character of variable is recorded
    if (recordingVariable) {
        if (variableName == "while") { res.emplace_back(TokenType::WHILE, variableName, line, index - variableLength, -1); }
        else if (variableName == "if") { res.emplace_back(TokenType::IF, variableName, line, index - variableLength, -1); }
        else if (variableName == "else") { res.emplace_back(TokenType::ELSE, variableName, line, index - variableLength, -1); }
        else if (variableName == "true") { res.emplace_back(TokenType::TRUE, variableName, line, index - variableLength, 1); }
        else if (variableName == "false") { res.emplace_back(TokenType::FALSE, variableName, line, index - variableLength, 0); }
        else if (variableName == "print") { res.emplace_back(TokenType::PRINT, variableName, line, index - variableLength, -1); }
        else if (variableName == "def") { res.emplace_back(TokenType::DEF, variableName, line, index - variableLength, -1); }
        else if (variableName == "null") { res.emplace_back(TokenType::NUL, variableName, line, index - variableLength, -1); }
        else if (variableName == "return") { res.emplace_back(TokenType::RETURN, variableName, line, index - variableLength, -1); }
        else { res.emplace_back(TokenType::VARIABLE, variableName, line, index - variableLength, -1); }
    }


    res.emplace_back(TokenType::END, "END", line, index, -1);

    return { -1, -1 };
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
    std::vector<Token> TokenVector;
    GenTokenVector(input, TokenVector);
    Token::printLexer(TokenVector);
}
