#pragma once
#include "ASTNode.h"
#include "Scope.h"

class Function
{
public:
    Function() = default;

    void setScope(Scope& scope) {
        m_CaptureScope = Scope(scope);

        for (std::string parameterName : m_ParameterNames)
        {
            m_CaptureScope.variableTypeMap.erase(parameterName);
        }
    }

    std::vector<std::string> m_ParameterNames;
    std::vector<std::unique_ptr<Node>> m_FunctionFlows;

    Scope m_CaptureScope;
};