#ifndef PARSER_H
#define PARSER_H

#include "lex.h"
#include <string>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    double parseExpression();
    std::string getStringExpression();
private:

    const std::vector<Token>& tokens_;
    int current;
    void checkSymbol(const std::string symbol);
    std::string stringParse;
    Token getNextToken();
    double applyOperator(const std::string& op, const std::vector<double>& args);
};

#endif




