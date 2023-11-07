#include "lib/Lexer.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <memory>
#include <stack>
class Node
{
public:
    Node() : value(Token(TokenType::NONE, "", -1, -1, -1)) {}
    Node(Token token) : value(token) {}
    virtual ~Node() = default;

    Token value;
};

class Function
{
public:
    Function() = default;
};

class Scope
{
public:
    Scope()
    : variableFunctionMap()
    {}

    std::map<std::string, std::shared_ptr<Function>> variableFunctionMap;
};

class Result 
{
public:
    ~Result() {
    }

    Function* function;
};


int main() {

    std::stack<Scope> ScopeStack = std::stack<Scope>();
    ScopeStack.push(Scope());
    auto& variableFunctionMap = ScopeStack.top().variableFunctionMap;

    // // std::map<std::string, std::shared_ptr<Function>> variableFunctionMap = std::map<std::string, std::shared_ptr<Function>>();

    // Result result = Result();
    // result.function = new Function();

    // // variableFunctionMap["add"] = std::make_shared<Function>();
    // // variableFunctionMap["add"].reset(result.function);
    // std::shared_ptr<Function> p = std::make_shared<Function>();
    // p.reset(result.function);

    Result result = Result();
    result.function = new Function();
    variableFunctionMap["add"] = std::make_shared<Function>();
    variableFunctionMap["add"].reset(result.function);

    return 0;
}