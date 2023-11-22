#pragma once

#include <map>
#include <memory>
#include "DataType.h"
#include "ASTNode.h"

class Function;

class Scope
{
public:
    Scope()
    : variableTypeMap(), variableDoubleMap(), variableBoolMap()
    , variableFunctionMap(), variableArrayMap()
    {}

    // Copy constructor
    Scope(const Scope& other)
    : variableTypeMap(other.variableTypeMap), variableDoubleMap(other.variableDoubleMap),
        variableBoolMap(other.variableBoolMap), variableFunctionMap(other.variableFunctionMap), variableArrayMap(other.variableArrayMap)
    {}

    ~Scope()
    {
        for (auto& pair : variableFunctionMap) {
            pair.second.reset();
        }
        // std::cout << "Scope is deleted" << std::endl;
    }

        // Copy assignment operator
    Scope& operator=(const Scope& other) {
        if (this != &other) {
            // Implement the assignment logic here
            // Be sure to handle any dynamic resources properly
            variableTypeMap = other.variableTypeMap;
            variableDoubleMap = other.variableDoubleMap;
            variableBoolMap = other.variableBoolMap;
            variableFunctionMap = other.variableFunctionMap;
        }
        return *this;
    }

    void OverwriteBy(Scope& other)
    {
        for (std::pair<std::string, DataType> kv : other.variableTypeMap)
        {
            std::string variableName = kv.first;
            variableTypeMap[variableName] = other.variableTypeMap[variableName];

            if (other.variableTypeMap[variableName] == DataType::NUL)
            {

            }
            else if (other.variableTypeMap[variableName] == DataType::UNINITIALIZED)
            {

            }
            else if (other.variableTypeMap[variableName] == DataType::DOUBLE)
            {
                variableDoubleMap[variableName] = other.variableDoubleMap[variableName];
            }
            else if (other.variableTypeMap[variableName] == DataType::BOOL)
            {
                variableBoolMap[variableName] = other.variableBoolMap[variableName];
            }        
            else if (other.variableTypeMap[variableName] == DataType::FUNCTION)
            {
                variableFunctionMap[variableName] = other.variableFunctionMap[variableName];
            }
            else
            {
                // std::cout << variableName << std::endl;
                // std::cout << "There is a problem overwrite scope" << std::endl;
            }
        }
    }

    void print()
    {
        std::cout << "======================" << std::endl;
        for (std::pair<std::string, DataType> kv : variableTypeMap)
        {
            std::string variableName = kv.first;

            if (variableTypeMap[variableName] == DataType::NUL)
            {
                std::cout << variableName << " null" << std::endl;
            }
            else if (variableTypeMap[variableName] == DataType::UNINITIALIZED)
            {
                std::cout << variableName << " uninitialized" << std::endl;
            }
            else if (variableTypeMap[variableName] == DataType::DOUBLE)
            {
                std::cout << variableName << " double " << variableDoubleMap[variableName] << std::endl;
            }
            else if (variableTypeMap[variableName] == DataType::BOOL)
            {
                std::cout << variableName << " bool " << variableBoolMap[variableName] << std::endl;
            }        
            else if (variableTypeMap[variableName] == DataType::FUNCTION)
            {
                std::cout << variableName << " function " << std::endl;
            }
            else
            {
                // std::cout << variableName << std::endl;
                // std::cout << "There is a problem overwrite scope" << std::endl;
            }
        }
        std::cout << "--------------------" << std::endl;
    }

    std::map<std::string, DataType> variableTypeMap;
    std::map<std::string, double> variableDoubleMap;
    std::map<std::string, bool> variableBoolMap;
    std::map<std::string, std::shared_ptr<Function>> variableFunctionMap;
    std::map<std::string, std::shared_ptr<Node>> variableArrayMap;
};