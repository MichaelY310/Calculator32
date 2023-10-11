#include "lib/parse.h"
#include "lib/lex.h"
#include <vector>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens), current(-1), stringParse("") {
	if (tokens[0].type != TokenType::leftParenthesis) {
		std::cout << "Unexpected token at line " + std::to_string(tokens_[0].line) + " colemn " + std::to_string(tokens_[0].index) +": " + tokens_[0].content << std::endl;
		exit(2);
	}
}

double Parser::parseExpression() {
    Token token = getNextToken();

    if (token.type == TokenType::error) {
        std::cout << "Unexpected token at line " + std::to_string(tokens_[current].line) + " colemn " + std::to_string(tokens_[current].index) +": " + tokens_[current].content << std::endl;
        exit(2);
    }

    if (token.type == TokenType::leftParenthesis) {
        stringParse = stringParse + "(";
        std::string op = getNextToken().content;
        checkSymbol(op);
        std::vector<double> args;

        while (true) {
            if (tokens_[current + 1].type == TokenType::rightParenthesis) {
                break;
            }
            double arg = parseExpression();
            args.push_back(arg);
        }
        double result = applyOperator(op, args);
        stringParse = stringParse + ")";
        return result;
    }
    else if (token.type == TokenType::number) {
        return token.value;
    }
    else {
        std::cout << "Unexpected token at line " + std::to_string(tokens_[current].line) + " colemn " + std::to_string(tokens_[current].index) +": " + tokens_[current].content << std::endl;
        exit(2);
    }
}

std::string Parser::getStringExpression()
{
	return stringParse;
}

void Parser::checkSymbol(const std::string symbol)
{
    if (!(symbol == "+" || symbol == "-" || symbol == "*" || symbol == "/")) {
        std::cout << "Unexpected token at line " + std::to_string(tokens_[current].line) + " colemn " + std::to_string(tokens_[current].index) +": " + tokens_[current].content << std::endl;
        exit(2);
    }
}

Token Parser::getNextToken()
{
	current++;
	return tokens_[current];
}

double Parser::applyOperator(const std::string& op, const std::vector<double>& args) {
    if (args.empty()) {
        std::cout << "Unexpected token at line " + std::to_string(tokens_[current+1].line) + " colemn " + std::to_string(tokens_[current+1].index) +": " + tokens_[current + 1].content << std::endl;
        exit(2);
    }
    if (op == "+") {
        double sum = 0.0;
        for (size_t i = 0; i < args.size(); ++i) {
            sum += args[i];
            if (i != args.size() - 1) {
                stringParse = stringParse + std::to_string(args[i]) + " + ";
            }
            else {
                stringParse = stringParse + std::to_string(args[i]);
            }
        }
        return sum;
    }
    else if (op == "-") {
        double result = args[0];
        for (size_t i = 1; i < args.size(); ++i) {
            result -= args[i];
        }
        return result;
    }
    else if (op == "*") {
        double product = 1.0;
        for (double arg : args) {
            product *= arg;
        }
        return product;
    }
    else {
        double result = args[0];
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == 0.0) {
                std::cerr << "Runtime error: division by zero." << std::endl;
                exit(3);
            }
            result /= args[i];
        }
        return result;
    }
}

