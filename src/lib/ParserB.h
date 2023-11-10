#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <memory>
#include <stack>

#include "DataType.h"
#include "Lexer.h"
#include "Scope.h"
#include "Function.h"
#include "ASTNode.h"



class Result {
public:
    ~Result() {
        if (type == DataType::FUNCTION && function != nullptr) {
            // delete function;
            // function = nullptr;
        }
    }

    DataType type;
    bool isreturn = false;
    std::shared_ptr<Function> function;
    union {
        double doubleValue;
        bool boolValue;
    };
};

class ParserB
{
public:
    static std::pair<std::pair<int, int>, std::string> HandleTokenVector(std::vector<Token> tokenVector, int leftBound, int rightBound, std::vector<std::unique_ptr<Node>>& nodes);
    static std::pair<std::pair<int, int>, std::string> MakeExpressionTree(std::vector<Token> expression, int leftBound, int rightBound, std::unique_ptr<ExpressionNode>& node);
    static std::string calculate(Node* root, Result& result);
    static void print(Node* root, int indent = 0);
    static void printValue(Result& result);
    static void setupExpression(std::vector<Token>& expression);
    static void getVariable(std::string& variableName, Result& result);
    static void setVariable(std::string& variableName, Result& result);
    static int findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightBracketNoError(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightBraceNoError(std::vector<Token> expression, int leftBound, int rightBound);

    static std::stack<Scope> ScopeStack;
    static std::map<TokenType, int> hierarchyMap;
private:
};

