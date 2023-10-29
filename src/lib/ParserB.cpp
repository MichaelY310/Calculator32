#include "ParserB.h"

// Initialize static member variables
std::map<std::string, double> ParserB::variableMap = std::map<std::string, double>();
std::map<std::string, bool> ParserB::variableInitializedMap = std::map<std::string, bool>();
std::map<std::string, DataType> ParserB::variableTypeMap = std::map<std::string, DataType>();
std::map<TokenType, int> ParserB::hierarchyMap = {
    {TokenType::NUMBER ,          0}, // 1 2 3
    {TokenType::TRUE ,            0}, // true
    {TokenType::FALSE ,           0}, // false
    {TokenType::VARIABLE ,        0}, // a b c
    {TokenType::LEFT_PARENTHESIS ,1}, // ()
    {TokenType::MULTIPLY ,        2}, // *
    {TokenType::DIVIDE ,          2}, // /
    {TokenType::MOD ,             2}, // %
    {TokenType::PLUS ,            3}, // +
    {TokenType::MINUS ,           3}, // -
    {TokenType::SMALLER ,         4}, // <
    {TokenType::BIGGER ,          4}, // >
    {TokenType::SMALLER_EQUAL ,   4}, // <=
    {TokenType::BIGGER_EQUAL ,    4}, // >=
    {TokenType::EQUALITY ,        5}, // ==
    {TokenType::INEQUALITY ,      5}, // !=
    {TokenType::AND ,             6}, // &
    {TokenType::EXCLUSIVE_OR ,    7}, // ^
    {TokenType::INCLUSIVE_OR ,    8}, // |
    {TokenType::ASSIGNMENT ,      9}  // =
};

// takes in a node* vector and add elements into it given the tokenVector
std::pair<std::pair<int, int>, std::string> ParserB::HandleTokenVector(std::vector<Token> tokenVector, int leftBound, int rightBound, std::vector<std::unique_ptr<Node>>& nodes)
{
    int start = leftBound;
    while (start < rightBound)
    {
        if (tokenVector[start].type == TokenType::WHILE)
        {
            std::unique_ptr<WhileNode> node = std::make_unique<WhileNode>(tokenVector[start]);

            // find where { is
            int leftBracketIndex = start + 1;
            while (leftBracketIndex < rightBound)
            {
                if (tokenVector[leftBracketIndex].type == TokenType::LEFT_BRACKET) { break; }
                leftBracketIndex += 1;
            }
            // { not found
            if (leftBracketIndex == rightBound)
            {
                return { { tokenVector[leftBracketIndex].line, tokenVector[leftBracketIndex].index }, tokenVector[leftBracketIndex].content };
            }
            // find }
            int rightBracketIndex = findRightBracketNoError(tokenVector, leftBracketIndex + 1, rightBound);
            if (rightBracketIndex > rightBound)
            {
                return { { tokenVector[rightBracketIndex].line, tokenVector[rightBracketIndex].index }, tokenVector[rightBracketIndex].content };  
            }

            // condition
            std::pair<std::pair<int, int>, std::string> errorResultCondition = MakeExpressionTree(tokenVector, start + 1, leftBracketIndex - 1, node->condition);
            if (errorResultCondition.first.first != -1) 
            {
                return errorResultCondition;
            }

            // statements and expressions inside {...}
            std::pair<std::pair<int, int>, std::string> errorResultFlows = HandleTokenVector(tokenVector, leftBracketIndex + 1, rightBracketIndex - 1, node->flows);
            if (errorResultFlows.first.first != -1) 
            {
                return errorResultFlows;
            }

            nodes.push_back(std::move(node));
            start = rightBracketIndex + 1;
        }
        else if (tokenVector[start].type == TokenType::IF)
        {
            std::unique_ptr<IfNode> node = std::make_unique<IfNode>(tokenVector[start]);

            // if
            // find {
            int leftBracketIndex = start + 1;
            while (leftBracketIndex < rightBound)
            {
                if (tokenVector[leftBracketIndex].type == TokenType::LEFT_BRACKET) { break; }
                leftBracketIndex += 1;
            }
            // ERROR: { not found
            if (leftBracketIndex == rightBound)
            { return { { tokenVector[leftBracketIndex].line, tokenVector[leftBracketIndex].index }, tokenVector[leftBracketIndex].content }; }
            // find }
            int rightBracketIndex = findRightBracketNoError(tokenVector, leftBracketIndex + 1, rightBound);
            // ERROR: } not found
            if (rightBracketIndex > rightBound)
            { return { { tokenVector[rightBracketIndex].line, tokenVector[rightBracketIndex].index }, tokenVector[rightBracketIndex].content }; }

            // condition
            std::unique_ptr<ExpressionNode> condition1 = std::make_unique<ExpressionNode>();
            std::pair<std::pair<int, int>, std::string> errorResultCondition1 = MakeExpressionTree(tokenVector, start + 1, leftBracketIndex - 1, condition1);
            if (errorResultCondition1.first.first != -1) { return errorResultCondition1; }

            // statements and expressions inside {...}
            std::vector<std::unique_ptr<Node>> flows1;
            std::pair<std::pair<int, int>, std::string> errorResultFlows1 = HandleTokenVector(tokenVector, leftBracketIndex + 1, rightBracketIndex - 1, flows1);
            if (errorResultFlows1.first.first != -1)  { return errorResultFlows1; }
            node->conditions.push_back(std::move(condition1));
            node->flowGroups.push_back(std::move(flows1));
            start = rightBracketIndex + 1;


            // Else
            if (tokenVector[start].type == TokenType::ELSE)
            {
                // Case 1 else {...}
                if (tokenVector[start+1].type == TokenType::LEFT_BRACKET)
                {
                    // find where { is
                    leftBracketIndex = start + 1;
                    // find }
                    rightBracketIndex = findRightBracketNoError(tokenVector, leftBracketIndex + 1, rightBound);
                    // ERROR: } not found
                    if (rightBracketIndex > rightBound)
                    { return { { tokenVector[rightBracketIndex].line, tokenVector[rightBracketIndex].index }, tokenVector[rightBracketIndex].content }; }
                    
                    // statements and expressions inside {...}
                    std::vector<std::unique_ptr<Node>> flows;
                    std::pair<std::pair<int, int>, std::string> errorResultFlows = HandleTokenVector(tokenVector, leftBracketIndex + 1, rightBracketIndex - 1, flows);
                    if (errorResultFlows.first.first != -1) { return errorResultFlows; }
                    node->flowGroups.push_back(std::move(flows));
                    start = rightBracketIndex + 1;
                }
                // Case 2 else if {...}
                else if (tokenVector[start+1].type == TokenType::IF)
                {
                    int newstart = start + 1;   // if
                    // Find the end of the whole if statement
                    // skip all else_if
                    while (tokenVector[start].type == TokenType::ELSE && tokenVector[start + 1].type == TokenType::IF)
                    {
                        // find where { is
                        int leftBracketIndex = start + 2;
                        while (leftBracketIndex < rightBound)
                        {
                            if (tokenVector[leftBracketIndex].type == TokenType::LEFT_BRACKET) { break; }
                            leftBracketIndex += 1;
                        }
                        // { not found
                        if (leftBracketIndex == rightBound)
                        { 
                            // std::cout << "{ not found" << std::endl;
                            return { { tokenVector[leftBracketIndex].line, tokenVector[leftBracketIndex].index }, tokenVector[leftBracketIndex].content }; 
                        }
                        // find }
                        int rightBracketIndex = findRightBracketNoError(tokenVector, leftBracketIndex + 1, rightBound);
                        if (rightBracketIndex > rightBound)
                        { 
                            // std::cout << "} not found" << std::endl;
                            return { { tokenVector[rightBracketIndex].line, tokenVector[rightBracketIndex].index }, tokenVector[rightBracketIndex].content }; 
                        }

                        start = rightBracketIndex + 1;
                    }
                    // skip else  (if there is one)
                    if (tokenVector[start].type == TokenType::ELSE)
                    {
                        // find where { is
                        // the last else must be "else", not else if
                        if (tokenVector[start+1].type != TokenType::LEFT_BRACKET)
                        {
                            return { { tokenVector[start+1].line, tokenVector[start+1].index }, tokenVector[start+1].content };  
                        }
                        leftBracketIndex = start + 1;
                        // find }
                        rightBracketIndex = findRightBracketNoError(tokenVector, leftBracketIndex + 1, rightBound);
                        // ERROR: } not found
                        if (rightBracketIndex > rightBound)
                        { return { { tokenVector[rightBracketIndex].line, tokenVector[rightBracketIndex].index }, tokenVector[rightBracketIndex].content }; }
                        
                        start = rightBracketIndex + 1;
                    }



                    // statements and expressions after else
                    std::vector<std::unique_ptr<Node>> flows;
                    std::pair<std::pair<int, int>, std::string> errorResultFlows = HandleTokenVector(tokenVector, newstart, start - 1, flows);
                    if (errorResultFlows.first.first != -1) { return errorResultFlows; }
                    node->flowGroups.push_back(std::move(flows));
                }
                else
                {
                    return { { tokenVector[start+1].line, tokenVector[start+1].index }, tokenVector[start+1].content };  
                }
            }
            nodes.push_back(std::move(node));
        }
        else if (tokenVector[start].type == TokenType::PRINT)
        {
            std::unique_ptr<PrintNode> node = std::make_unique<PrintNode>(tokenVector[start]);
            int printIndex = start;
            start += 1;
            while (tokenVector[start].line == tokenVector[printIndex].line && start <= rightBound)
            {
                start += 1;
            }
            auto errorResult = MakeExpressionTree(tokenVector, printIndex + 1, start - 1, node->content);
            if (errorResult.first.first != -1) 
            {
                return errorResult;
            }
            nodes.push_back(std::move(node));
        }
        else
        {
            std::unique_ptr<ExpressionNode> node = std::make_unique<ExpressionNode>(tokenVector[start]);
            int beginIndex = start;
            start += 1;
            while (tokenVector[start].line == tokenVector[beginIndex].line && start <= rightBound)
            {
                start += 1;
            }
            auto errorResult = MakeExpressionTree(tokenVector, beginIndex, start - 1, node);
            if (errorResult.first.first != -1) 
            {
                return errorResult;
            }
            nodes.push_back(std::move(node));
        }
    }
    return { { -1, -1 }, "" };
}

std::pair<std::pair<int, int>, std::string> ParserB::MakeExpressionTree(std::vector<Token> expression, int leftBound, int rightBound, std::unique_ptr<ExpressionNode>& node)
{ 
    if (leftBound > rightBound) {
#if DEBUG
    std::cout << "1  no expression  " << std::endl;
#endif
        return { { expression[leftBound].line, expression[leftBound].index }, expression[leftBound].content };
    }


    // Find the top level token
    int topIndex = leftBound;
    int i = leftBound + 1;
    // skip parenthesis content for the first element
    if (expression[topIndex].type == TokenType::LEFT_PARENTHESIS)
    {
        int rightIndex = findRightParenthesisNoError(expression, topIndex+1, rightBound);
        if (rightIndex > rightBound)
        {
            return { { expression[rightIndex].line, expression[rightIndex].index }, expression[rightIndex].content };  
        }
        i = rightIndex + 1;
    }


    while (i <= rightBound)
    {
        // current token does not belongs to hierarchyMap
        if (hierarchyMap.find(expression[i].type) == hierarchyMap.end())
        {
#if DEBUG
    std::cout << "2  unknown token  " << std::endl;
#endif
            return { { expression[i].line, expression[i].index }, expression[i].content };
        } 
        // top level token does not belongs to hierarchyMap
        if (hierarchyMap.find(expression[topIndex].type) == hierarchyMap.end())
        {
#if DEBUG
    std::cout << "1.5  the first token unknown token  " << std::endl;
#endif
            return { { expression[topIndex].line, expression[topIndex].index }, expression[topIndex].content };
        } 

        // higher hierarchy
        if (hierarchyMap.at(expression[i].type) > hierarchyMap.at(expression[topIndex].type))
        {
            topIndex = i;
        } 
        // same hierarchy
        else if (hierarchyMap.at(expression[i].type) == hierarchyMap.at(expression[topIndex].type))
        {
            // = right associative
            if (expression[i].type == TokenType::ASSIGNMENT)
            {
                topIndex = topIndex < i ? topIndex : i;
            }
            // left associative
            else
            {
                topIndex = topIndex > i ? topIndex : i;
            }
        }
        // lower hierarchy
        else
        {
            // do nothing
        }

        // Find the next element
        // ignore content inside (...)
        if (expression[i].type == TokenType::LEFT_PARENTHESIS)
        {
            int rightIndex = findRightParenthesisNoError(expression, i+1, rightBound);
            if (rightIndex > rightBound)
            {
                return { { expression[rightIndex].line, expression[rightIndex].index }, expression[rightIndex].content };  
            }
            i = rightIndex + 1;
        }
        else
        {
            i += 1;
        } 
    }

    // case 1 number
    if (expression[topIndex].type == TokenType::NUMBER)
    {
        node = std::make_unique<ExpressionNode>(expression[topIndex]);
        return { { -1, -1 }, "" };
    }
    // case 2 variable
    else if (expression[topIndex].type == TokenType::VARIABLE)
    {
        node = std::make_unique<ExpressionNode>(expression[topIndex]);
        return { { -1, -1 }, "" };
    }
    // case 3 true
    else if (expression[topIndex].type == TokenType::TRUE)
    {
        node = std::make_unique<ExpressionNode>(expression[topIndex]);
        return { { -1, -1 }, "" };
    }
    // case 4 false
    else if (expression[topIndex].type == TokenType::FALSE)
    {
        node = std::make_unique<ExpressionNode>(expression[topIndex]);
        return { { -1, -1 }, "" };
    }
    // case 5 =
    else if (expression[topIndex].type == TokenType::ASSIGNMENT)
    {
        node = std::make_unique<ExpressionNode>(expression[topIndex]);

        // check assignment ERROR
        // Error 1. Nothing before =   e.g =8
        if (topIndex == leftBound)
            return { { expression[topIndex].line, expression[topIndex].index }, expression[topIndex].content }; 
        // Error 2. Nothing after =   e.g a=
        if (topIndex == rightBound)
            return { { expression[topIndex+1].line, expression[topIndex+1].index }, expression[topIndex+1].content }; 
        // Error 3. What is before = is not a variable      e.g 1=1
        if (expression[topIndex-1].type != TokenType::VARIABLE)
            return { { expression[topIndex].line, expression[topIndex].index }, expression[topIndex].content };  

        // on the left
        std::unique_ptr<ExpressionNode> node1;
        std::pair<std::pair<int, int>, std::string> errorResult1 = MakeExpressionTree(expression, leftBound, topIndex-1, node1);
        if (errorResult1.first.first != -1) { return errorResult1; }
        node->children.push_back(std::move(node1));

        // on the right
        std::unique_ptr<ExpressionNode> node2;
        std::pair<std::pair<int, int>, std::string> errorResult2 = MakeExpressionTree(expression, topIndex+1, rightBound, node2);
        if (errorResult2.first.first != -1) { return errorResult2; }
        node->children.push_back(std::move(node2));

        return { { -1, -1 }, "" };
    }
    // case 6 + - * /
    else if (expression[topIndex].type == TokenType::PLUS || expression[topIndex].type == TokenType::MINUS || 
            expression[topIndex].type == TokenType::MULTIPLY || expression[topIndex].type == TokenType::DIVIDE || 
            expression[topIndex].type == TokenType::MOD ||
            expression[topIndex].type == TokenType::SMALLER || expression[topIndex].type == TokenType::BIGGER|| 
            expression[topIndex].type == TokenType::SMALLER_EQUAL || expression[topIndex].type == TokenType::BIGGER_EQUAL || 
            expression[topIndex].type == TokenType::INEQUALITY || expression[topIndex].type == TokenType::EQUALITY ||
            expression[topIndex].type == TokenType::AND || expression[topIndex].type == TokenType::EXCLUSIVE_OR || expression[topIndex].type == TokenType::INCLUSIVE_OR)
    {
        node = std::make_unique<ExpressionNode>(expression[topIndex]);

        // on the left
        std::unique_ptr<ExpressionNode> node1;
        std::pair<std::pair<int, int>, std::string> errorResult1 = MakeExpressionTree(expression, leftBound, topIndex-1, node1);
        if (errorResult1.first.first != -1) { return errorResult1; }
        node->children.push_back(std::move(node1));

        // on the right
        std::unique_ptr<ExpressionNode> node2;
        std::pair<std::pair<int, int>, std::string> errorResult2 = MakeExpressionTree(expression, topIndex+1, rightBound, node2);
        if (errorResult2.first.first != -1) { return errorResult2; }
        node->children.push_back(std::move(node2));
        
        return { { -1, -1 }, "" };
    }
    // case 7 (...)
    else if (expression[topIndex].type == TokenType::LEFT_PARENTHESIS)
    {
        int rightIndex = findRightParenthesisNoError(expression, topIndex+1, rightBound);
        if (rightIndex > rightBound)
        {
            return { { expression[rightIndex].line, expression[rightIndex].index }, expression[rightIndex].content };  
        }
        // get rid of the parenthesis pair
        return MakeExpressionTree(expression, topIndex+1, rightIndex-1, node);
    }
    // case 8 ERROR
    else
    {
#if DEBUG
    std::cout << "3   " << std::endl;
#endif
        return { { expression[topIndex].line, expression[topIndex].index }, expression[topIndex].content };
    }
}

std::string ParserB::calculate(Node* root, double& result, DataType& resultType)
{
    // While
    if (root->value.type == TokenType::WHILE)
    {
        WhileNode* whileNode = dynamic_cast<WhileNode*>(root);
        while (true)
        {
            // check condition
            double conditionResult;
            DataType conditionResultType;
            std::string errorMessageCondition = calculate(whileNode->condition.get(), conditionResult, conditionResultType);
            // ERROR: Condition is not a bool
            if (conditionResultType != DataType::BOOL) { return "Runtime error: condition is not a bool."; }
            if (errorMessageCondition != "") { return errorMessageCondition; }
            if (conditionResult == 0) { break; }
            // execute flows
            for (int i=0; i < (int)whileNode->flows.size(); i++)
            {
                double flowResult;
                DataType flowResultType;
                std::string errorMessageFlow = calculate(whileNode->flows[i].get(), flowResult, flowResultType);
                if (errorMessageFlow != "") { return errorMessageFlow; }            
            }
        }
        return "";
    }
    // If
    else if (root->value.type == TokenType::IF)
    {
        IfNode* ifNode = dynamic_cast<IfNode*>(root);
        for (int conditionIndex = 0; conditionIndex < (int)ifNode->conditions.size(); conditionIndex++)
        {
            // check conditions
            double conditionResult;
            DataType conditionResultType;
            std::string errorMessageCondition = calculate(ifNode->conditions[conditionIndex].get(), conditionResult, conditionResultType);
            // ERROR: Condition is not a bool
            if (conditionResultType != DataType::BOOL) { return "Runtime error: condition is not a bool."; }
            if (errorMessageCondition != "") { return errorMessageCondition; }            
            if (conditionResult == 1) 
            {
                // execute flows
                for (int i=0; i < (int)ifNode->flowGroups[conditionIndex].size(); i++)
                {
                    double flowResult;
                    DataType flowResultType;
                    std::string errorMessageFlow = calculate(ifNode->flowGroups[conditionIndex][i].get(), flowResult, flowResultType);
                    if (errorMessageFlow != "") { return errorMessageFlow; }            
                }
                return "";
            }

        }

        // check else
        if (ifNode->conditions.size() != ifNode->flowGroups.size())
        {
            for (int i=0; i < (int)ifNode->flowGroups[ifNode->flowGroups.size()-1].size(); i++)
            {
                double flowResult;
                DataType flowResultType;
                std::string errorMessageFlow = calculate(ifNode->flowGroups[ifNode->flowGroups.size()-1][i].get(), flowResult, flowResultType);
                if (errorMessageFlow != "") { return errorMessageFlow; }            
            }
            return "";
        }
        return "";
    }
    // Print
    else if (root->value.type == TokenType::PRINT)
    {
        PrintNode* printNode = dynamic_cast<PrintNode*>(root);
        double flowResult;
        DataType flowResultType;
        std::string errorMessageFlow = calculate(printNode->content.get(), flowResult, flowResultType);
        if (errorMessageFlow != "") { return errorMessageFlow; }            
        // std::cout << flowResult << std::endl;
        ParserB::printValue(flowResult, flowResultType);
        std::cout << std::endl;
    }

    // Expression
    else 
    {
        ExpressionNode* expressionNode = dynamic_cast<ExpressionNode*>(root);

        // number
        if (expressionNode->value.type == TokenType::NUMBER)
        {
            result = expressionNode->value.value;
            resultType = DataType::DOUBLE;
            return "";
        }

        // true
        else if (expressionNode->value.type == TokenType::TRUE)
        {
            result = expressionNode->value.value;
            resultType = DataType::BOOL;
            return "";
        }

        // false
        else if (expressionNode->value.type == TokenType::FALSE)
        {
            result = expressionNode->value.value;
            resultType = DataType::BOOL;
            return "";
        }

        // variable
        else if (expressionNode->value.type == TokenType::VARIABLE)
        {
            if (variableInitializedMap.at(expressionNode->value.content) == false)
            {
                return "Runtime error: unknown identifier " + expressionNode->value.content;
            }
            result = variableMap.at(expressionNode->value.content);
            resultType = variableTypeMap.at(expressionNode->value.content);
            return "";
        }

        // =
        else if (expressionNode->value.type == TokenType::ASSIGNMENT)
        {
            // the last child must be a number or an initialized variable
            std::string errorMessage = calculate(expressionNode->children[1].get(), result, resultType);
            if (errorMessage != "") { return errorMessage; }

            variableMap.at(expressionNode->children[0]->value.content) = result;
            variableInitializedMap.at(expressionNode->children[0]->value.content) = true;
            variableTypeMap.at(expressionNode->children[0]->value.content) = resultType;

            return "";
        }

        // + - * / % == != > < >= <= & | ^
        else {
            // variable for operation is uninitialaized
            double result1;
            double result2;
            DataType resultType1;
            DataType resultType2;
            std::string errorMessage1 = calculate(expressionNode->children[0].get(), result1, resultType1);
            if (errorMessage1 != "") { return errorMessage1; }
            std::string errorMessage2 = calculate(expressionNode->children[1].get(), result2, resultType2);
            if (errorMessage2 != "") { return errorMessage2; }


            // handle type errors and set resultType
            // arithematic operations
            // + - * / %
            if (expressionNode->value.type == TokenType::PLUS || expressionNode->value.type == TokenType::MINUS || expressionNode->value.type == TokenType::MULTIPLY || 
                expressionNode->value.type == TokenType::DIVIDE || expressionNode->value.type == TokenType::MOD) 
            {   
                // arithmetic operator only works on double and return double
                if (resultType1 == DataType::BOOL || resultType2 == DataType::BOOL)
                {
                    return "Runtime error: invalid operand type.";
                }
                resultType = DataType::DOUBLE;
            }
            // ordered comparison
            // < <= > >= 
            else if (expressionNode->value.type == TokenType::SMALLER || expressionNode->value.type == TokenType::SMALLER_EQUAL || 
                    expressionNode->value.type == TokenType::BIGGER  || expressionNode->value.type == TokenType::BIGGER_EQUAL) 
            { 
                // Comparsion operator only compare double and return bool
                if (resultType1 == DataType::BOOL || resultType2 == DataType::BOOL)
                {
                    return "Runtime error: invalid operand type.";
                }
                resultType = DataType::BOOL;
            }
            // logic operations
            // & | ^
            else if (expressionNode->value.type == TokenType::AND || expressionNode->value.type == TokenType::INCLUSIVE_OR || 
                    expressionNode->value.type == TokenType::EXCLUSIVE_OR) 
            {
                if (resultType1 != DataType::BOOL || resultType2 != DataType::BOOL)
                {
                    return "Runtime error: invalid operand type.";
                }
                resultType = DataType::BOOL;
            }
            // Equality and Inequality
            // == !=
            else if (expressionNode->value.type == TokenType::EQUALITY || expressionNode->value.type == TokenType::INEQUALITY)
            {
                // inequality or equality can works on both double or bool but only return bool
                if (resultType1 != resultType2)
                {
                    return "Runtime error: invalid operand type.";
                }
                resultType = DataType::BOOL;
            }


            // + - * /
            if (expressionNode->value.type == TokenType::PLUS) {
                result = result1 + result2; 
            }
            else if (expressionNode->value.type == TokenType::MINUS) {
                result = result1 - result2; 
            }
            else if (expressionNode->value.type == TokenType::MULTIPLY) { 
                result = result1 * result2; 
            }
            else if (expressionNode->value.type == TokenType::DIVIDE)
            {
                if (result2 == 0) {
                    return "Runtime error: division by zero.";
                }
                result = result1 / result2; 
            }
            // %
            else if (expressionNode->value.type == TokenType::MOD) {
                result = std::fmod(result1, result2); 
            }
            // ==
            else if (expressionNode->value.type == TokenType::EQUALITY) {
                result = result1 == result2; 
            }
            // !=
            else if (expressionNode->value.type == TokenType::INEQUALITY) {
                result = result1 != result2; 
            }
            // >
            else if (expressionNode->value.type == TokenType::BIGGER) {
                result = result1 > result2; 
            }
            // <
            else if (expressionNode->value.type == TokenType::SMALLER) {
                result = result1 < result2; 
            }
            // >=
            else if (expressionNode->value.type == TokenType::BIGGER_EQUAL) {
                result = result1 >= result2; 
            }
            // <=
            else if (expressionNode->value.type == TokenType::SMALLER_EQUAL) {
                result = result1 <= result2; 
            }
            // &
            else if (expressionNode->value.type == TokenType::AND) {
                result = result1 && result2; 
            }
            // |
            else if (expressionNode->value.type == TokenType::INCLUSIVE_OR) {
                result = result1 || result2; 
            }
            // ^
            else if (expressionNode->value.type == TokenType::EXCLUSIVE_OR) {
                result = result1 != result2;
            }
            return "";
        }
    }
    return "";
}


void ParserB::print(Node* root, int indent)
{
    for (int i=0; i<indent; i++) { std::cout << "    "; }

    // While
    if (root->value.type == TokenType::WHILE)
    {
        WhileNode* whileRoot = dynamic_cast<WhileNode*>(root);

        std::cout << "while ";
        print(whileRoot->condition.get(), 0);
        std::cout << " {" << std::endl;
        for (int i=0;i < (int)whileRoot->flows.size(); i++)
        {
            print(whileRoot->flows[i].get(), indent + 1);
            std::cout << std::endl;
        }
        for (int i=0; i<indent; i++) { std::cout << "    "; }
        std::cout << "}";
    }
    // If
    else if (root->value.type == TokenType::IF)
    {
        IfNode* ifRoot = dynamic_cast<IfNode*>(root);

        // if
        std::cout << "if ";
        print(ifRoot->conditions[0].get(), 0);
        std::cout << " {" << std::endl;
        for (int i=0 ; i < (int)ifRoot->flowGroups[0].size(); i++)
        {
            print(ifRoot->flowGroups[0][i].get(), indent + 1);
            std::cout << std::endl;
        }
        for (int i=0; i<indent; i++) { std::cout << "    "; }
        std::cout << "}";

        // else if
        for (int conditionIndex=1; conditionIndex < (int)ifRoot->conditions.size(); conditionIndex++)
        {
            std::cout << std::endl;
            for (int i=0; i<indent; i++) { std::cout << "    "; }
            std::cout << "else if ";
            print(ifRoot->conditions[conditionIndex].get(), 0);
            std::cout << " {" << std::endl;
            for (int i=0; i < (int)ifRoot->flowGroups[conditionIndex].size(); i++)
            {
                print(ifRoot->flowGroups[conditionIndex][i].get(), indent + 1);
                std::cout << std::endl;
            }
            for (int a=0; a<indent; a++) { std::cout << "    "; }
            std::cout << "}";
        }

        // else
        if (ifRoot->conditions.size() != ifRoot->flowGroups.size())
        {
            std::cout << std::endl;
            for (int i=0; i<indent; i++) { std::cout << "    "; }
            std::cout << "else {" << std::endl;
            for (int i=0; i < (int)ifRoot->flowGroups[ifRoot->flowGroups.size()-1].size(); i++)
            {
                print(ifRoot->flowGroups[ifRoot->flowGroups.size()-1][i].get(), indent + 1);
                std::cout << std::endl;
            }
            for (int i=0; i<indent; i++) { std::cout << "    "; }
            std::cout << "}";
        }
    }
    // Print
    else if (root->value.type == TokenType::PRINT)
    {
        PrintNode* printRoot = dynamic_cast<PrintNode*>(root);

        std::cout << "print ";
        print(printRoot->content.get());
    }

    // ExpressionNode
    else 
    {
        ExpressionNode* expressionNode = dynamic_cast<ExpressionNode*>(root);

        if (expressionNode->value.type == TokenType::NUMBER)
        {
            int v = floor(expressionNode->value.value);
            // 1000.000  ->  1000
            if ((double)v == expressionNode->value.value)
            { std::cout << v; }
            else
            { std::cout << expressionNode->value.content; }
        }
        else if (expressionNode->value.type == TokenType::VARIABLE)
        {
            std::cout << expressionNode->value.content;
        }
        else if (expressionNode->value.type == TokenType::TRUE)
        {
            std::cout << expressionNode->value.content;
        }
        else if (expressionNode->value.type == TokenType::FALSE)
        {
            std::cout << expressionNode->value.content;
        }
        else {
            std::cout << "(";
            for (int i = 0; i < (int)expressionNode->children.size(); i++)
            {
                print(expressionNode->children[i].get());
                if (i != (int)expressionNode->children.size() - 1)
                {
                    std::cout << " " << expressionNode->value.content << " ";
                }
            }
            std::cout << ")";
        }
    }
}

void ParserB::printValue(double value, DataType valueType)
{
    if (valueType == DataType::BOOL)
    {
        if (value == 0) { std::cout << "false"; }
        else            { std::cout << "true"; }
    }

    else
    {
        std::cout << value;
    }
}


// exclude left parenthesis
int ParserB::findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound)
{
    int balance = 1;
    int p = leftBound;
    while (p <= rightBound) {
        if (expression[p].type == TokenType::LEFT_PARENTHESIS) { balance += 1; }
        else if (expression[p].type == TokenType::RIGHT_PARENTHESIS) { balance -= 1; }

        if (balance == 0) { break; }
        p += 1;
    }
    return p;
}

// exclude left bracket
int ParserB::findRightBracketNoError(std::vector<Token> expression, int leftBound, int rightBound)
{
    int balance = 1;
    int p = leftBound;
    while (p <= rightBound) {
        if (expression[p].type == TokenType::LEFT_BRACKET) { balance += 1; }
        else if (expression[p].type == TokenType::RIGHT_BRACKET) { balance -= 1; }

        if (balance == 0) { break; }
        p += 1;
    }
    return p;
}


// return vectors of lines in infix notation
// register variables in map 
void ParserB::setupExpression(std::vector<Token> expression)
{
    for (int i = 0; i < (int)expression.size()-1; i++)
    {
        Token token = expression[i];
        // handle new variable
        if (token.type == TokenType::VARIABLE)
        {
            if (variableMap.find(token.content) == variableMap.end())
            {
                variableMap.insert({ token.content, -1 });
                variableInitializedMap.insert({ token.content, false });
                variableTypeMap.insert({ token.content, DataType::UNINITIALIZED });
            }
        }
    }
}
