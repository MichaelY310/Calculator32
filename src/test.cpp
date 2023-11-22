#include "lib/Lexer.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <memory>
#include <stack>


class Function;

class Scope
{
public:
    Scope()
    : variableFunctionMap()
    {
        std::cout << "scope is created" << std::endl;
    }

    ~Scope()
    {
        for (auto& pair : variableFunctionMap) {
            pair.second.reset();
        }
        std::cout << "Scope is deleted" << std::endl;
    }

    Scope(const Scope& other)
    : variableFunctionMap(other.variableFunctionMap) 
    {
        std::cout << "scope copied" << std::endl;
    }

    Scope& operator=(const Scope& other) {
        if (this != &other) {
            variableFunctionMap = other.variableFunctionMap;
        }
        return *this;
    }

    std::map<std::string, std::shared_ptr<Function>> variableFunctionMap;
};


class Function
{
public:
    Function()
    {
        std::cout << "Function is created" << std::endl;
    };

    ~Function()
    {
        // for (auto& pair : m_CaptureScope.variableFunctionMap) {
        //     pair.second.reset();
        // }
        // delete m_CaptureScope;
        std::cout << "Function is deleted" << std::endl;
    }

    Scope* m_CaptureScope;
};


static std::stack<Scope*> ScopeStack;

int main() {    
    // Add a Function inside globalScope
    Scope* globalScope = new Scope();
    ScopeStack.push(globalScope);

    std::shared_ptr<Function> function = std::make_shared<Function>();
    globalScope->variableFunctionMap["add"] = function;
    // map 里面有一个pointer指向function

    function->m_CaptureScope = new Scope(*globalScope);
    // function 里面有一个pointer指向 scope, scope 里有个 map, map 里面有一个 pointer 指向 function

    delete function->m_CaptureScope;
    std::cout << "1111" << std::endl;
    delete globalScope;
    std::cout << "2222" << std::endl;



    return 0;
}