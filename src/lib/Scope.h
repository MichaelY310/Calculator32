#pragma once

#include <map>
#include <memory>
#include "DataType.h"

class Function;

class Scope
{
public:
    Scope()
    : variableTypeMap(), variableDoubleMap(), variableBoolMap()
    , variableFunctionMap()
    {}

    // Copy constructor
    Scope(const Scope& other)
    : variableTypeMap(other.variableTypeMap), variableDoubleMap(other.variableDoubleMap),
        variableBoolMap(other.variableBoolMap), variableFunctionMap(other.variableFunctionMap) 
    {}

    void OverwriteBy(Scope& other)
    {
        for (std::pair<std::string, DataType> kv : other.variableTypeMap)
        {
            std::string variableName = kv.first;

            variableTypeMap[variableName] = other.variableTypeMap[variableName];

            if (other.variableTypeMap[variableName] == DataType::NUL)
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

    std::map<std::string, DataType> variableTypeMap;
    std::map<std::string, double> variableDoubleMap;
    std::map<std::string, bool> variableBoolMap;
    std::map<std::string, std::shared_ptr<Function>> variableFunctionMap;
};