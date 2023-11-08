#pragma once
#include "ASTNode.h"
#include "Scope.h"

class Function
{
public:
    Function() = default;
    std::vector<std::string> m_ParameterNames;
    std::vector<std::unique_ptr<Node>> m_FunctionFlows;

    Scope m_CaptureScope;
};