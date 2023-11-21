#include "ParserB.h"

// Initialize static member variables
std::stack<Scope*> ParserB::ScopeStack;
std::vector<std::shared_ptr<Function>> ParserB::functionStorage;

std::map<TokenType, int> ParserB::hierarchyMap = {
    {TokenType::NUL ,          0}, // null
    {TokenType::NUMBER ,          0}, // 1 2 3
    {TokenType::TRUE ,            0}, // true
    {TokenType::FALSE ,           0}, // false
    {TokenType::VARIABLE ,        0}, // a b c
    {TokenType::LEFT_BRACKET ,    0}, // []
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
    // auto& variableTypeMap = ScopeStack.top()->variableTypeMap;
    if (leftBound == rightBound && tokenVector[leftBound].type == TokenType::VARIABLE)
    {
        std::unique_ptr<Node> node = std::make_unique<Node>(tokenVector[leftBound]);
        nodes.push_back(std::move(node));   
    }
    while (start < rightBound)
    {
        if (tokenVector[start].type == TokenType::SEMICOLON)
        {
            start += 1;
        }

        // std::cout << start << std::endl;
        else if (tokenVector[start].type == TokenType::DEF)
        {
            std::unique_ptr<FunctionDefineNode> node = std::make_unique<FunctionDefineNode>(tokenVector[start]);
            int defIndex = start;
            // Name
            int functionNameIndex = defIndex + 1;
            if (tokenVector[functionNameIndex].type != TokenType::VARIABLE) { std::cout << "there is something wrong. def not followed by variable" << std::endl; }

            // (...)
            int leftParenthesisIndex = functionNameIndex + 1;
            if (tokenVector[leftParenthesisIndex].type != TokenType::LEFT_PARENTHESIS) { std::cout << "there is something wrong. functionname not followed by (" << std::endl; }
            int rightParenthesisIndex = findRightParenthesisNoError(tokenVector, leftParenthesisIndex + 1, rightBound);
            if (rightParenthesisIndex > rightBound)
            {
                return { { tokenVector[rightParenthesisIndex].line, tokenVector[rightParenthesisIndex].index }, tokenVector[rightParenthesisIndex].content };  
            }

            // {...}
            int leftBraceIndex = rightParenthesisIndex + 1;
            if (tokenVector[leftBraceIndex].type != TokenType::LEFT_BRACE) { std::cout << "there is something wrong. in function def ) not followed by {" << std::endl; }
            int rightBraceIndex = findRightBraceNoError(tokenVector, leftBraceIndex + 1, rightBound);
            if (rightBraceIndex > rightBound)
            {
                return { { tokenVector[rightBraceIndex].line, tokenVector[rightBraceIndex].index }, tokenVector[rightBraceIndex].content };  
            }


            // Node Setup
            // Function name
            node->functionName = tokenVector[functionNameIndex].content;

            // Function Parameters
            int parameterIndex = leftParenthesisIndex + 1;
            while (parameterIndex < rightParenthesisIndex)
            {
                // std::cout << "111" << std::endl;
                if (tokenVector[parameterIndex].type == TokenType::COMMA)
                {
                    parameterIndex += 1;
                    continue;
                }
                if (tokenVector[parameterIndex].type != TokenType::VARIABLE)
                {
                    return { { tokenVector[parameterIndex].line, tokenVector[parameterIndex].index }, tokenVector[parameterIndex].content };  
                }
                node->parameterNames.push_back(tokenVector[parameterIndex].content);
                parameterIndex += 1;
            }

            // Function Flows
            std::pair<std::pair<int, int>, std::string> errorResultFlows = HandleTokenVector(tokenVector, leftBraceIndex + 1, rightBraceIndex - 1, node->flows);
            if (errorResultFlows.first.first != -1) 
            {
                return errorResultFlows;
            }

            nodes.push_back(std::move(node));
            start = rightBraceIndex + 1;
        }
        else if (tokenVector[start].type == TokenType::WHILE)
        {
            std::unique_ptr<WhileNode> node = std::make_unique<WhileNode>(tokenVector[start]);

            // find where { is
            int leftBraceIndex = start + 1;
            while (leftBraceIndex < rightBound)
            {
                if (tokenVector[leftBraceIndex].type == TokenType::LEFT_BRACE) { break; }
                leftBraceIndex += 1;
            }
            // { not found
            if (leftBraceIndex == rightBound)
            {
                return { { tokenVector[leftBraceIndex].line, tokenVector[leftBraceIndex].index }, tokenVector[leftBraceIndex].content };
            }
            // find }
            int rightBraceIndex = findRightBraceNoError(tokenVector, leftBraceIndex + 1, rightBound);
            if (rightBraceIndex > rightBound)
            {
                return { { tokenVector[rightBraceIndex].line, tokenVector[rightBraceIndex].index }, tokenVector[rightBraceIndex].content };  
            }

            // condition
            std::pair<std::pair<int, int>, std::string> errorResultCondition = MakeExpressionTree(tokenVector, start + 1, leftBraceIndex - 1, node->condition);
            if (errorResultCondition.first.first != -1) 
            {
                return errorResultCondition;
            }

            // statements and expressions inside {...}
            std::pair<std::pair<int, int>, std::string> errorResultFlows = HandleTokenVector(tokenVector, leftBraceIndex + 1, rightBraceIndex - 1, node->flows);
            if (errorResultFlows.first.first != -1) 
            {
                return errorResultFlows;
            }

            nodes.push_back(std::move(node));
            start = rightBraceIndex + 1;
        }
        else if (tokenVector[start].type == TokenType::IF)
        {
            std::unique_ptr<IfNode> node = std::make_unique<IfNode>(tokenVector[start]);

            // if
            // find {
            int leftBraceIndex = start + 1;
            while (leftBraceIndex < rightBound)
            {
                if (tokenVector[leftBraceIndex].type == TokenType::LEFT_BRACE) { break; }
                leftBraceIndex += 1;
            }
            // ERROR: { not found
            if (leftBraceIndex == rightBound)
            { return { { tokenVector[leftBraceIndex].line, tokenVector[leftBraceIndex].index }, tokenVector[leftBraceIndex].content }; }
            // find }
            int rightBraceIndex = findRightBraceNoError(tokenVector, leftBraceIndex + 1, rightBound);
            // ERROR: } not found
            if (rightBraceIndex > rightBound)
            { return { { tokenVector[rightBraceIndex].line, tokenVector[rightBraceIndex].index }, tokenVector[rightBraceIndex].content }; }

            // condition
            std::unique_ptr<ExpressionNode> condition1 = std::make_unique<ExpressionNode>();
            std::pair<std::pair<int, int>, std::string> errorResultCondition1 = MakeExpressionTree(tokenVector, start + 1, leftBraceIndex - 1, condition1);
            if (errorResultCondition1.first.first != -1) { return errorResultCondition1; }

            // statements and expressions inside {...}
            std::vector<std::unique_ptr<Node>> flows1;
            std::pair<std::pair<int, int>, std::string> errorResultFlows1 = HandleTokenVector(tokenVector, leftBraceIndex + 1, rightBraceIndex - 1, flows1);
            if (errorResultFlows1.first.first != -1)  { return errorResultFlows1; }
            node->conditions.push_back(std::move(condition1));
            node->flowGroups.push_back(std::move(flows1));
            start = rightBraceIndex + 1;


            // Else
            if (tokenVector[start].type == TokenType::ELSE)
            {
                // Case 1 else {...}
                if (tokenVector[start+1].type == TokenType::LEFT_BRACE)
                {
                    // find where { is
                    leftBraceIndex = start + 1;
                    // find }
                    rightBraceIndex = findRightBraceNoError(tokenVector, leftBraceIndex + 1, rightBound);
                    // ERROR: } not found
                    if (rightBraceIndex > rightBound)
                    { return { { tokenVector[rightBraceIndex].line, tokenVector[rightBraceIndex].index }, tokenVector[rightBraceIndex].content }; }
                    
                    // statements and expressions inside {...}
                    std::vector<std::unique_ptr<Node>> flows;
                    std::pair<std::pair<int, int>, std::string> errorResultFlows = HandleTokenVector(tokenVector, leftBraceIndex + 1, rightBraceIndex - 1, flows);
                    if (errorResultFlows.first.first != -1) { return errorResultFlows; }
                    node->flowGroups.push_back(std::move(flows));
                    start = rightBraceIndex + 1;
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
                        int leftBraceIndex = start + 2;
                        while (leftBraceIndex < rightBound)
                        {
                            if (tokenVector[leftBraceIndex].type == TokenType::LEFT_BRACE) { break; }
                            leftBraceIndex += 1;
                        }
                        // { not found
                        if (leftBraceIndex == rightBound)
                        { 
                            // std::cout << "{ not found" << std::endl;
                            return { { tokenVector[leftBraceIndex].line, tokenVector[leftBraceIndex].index }, tokenVector[leftBraceIndex].content }; 
                        }
                        // find }
                        int rightBraceIndex = findRightBraceNoError(tokenVector, leftBraceIndex + 1, rightBound);
                        if (rightBraceIndex > rightBound)
                        { 
                            // std::cout << "} not found" << std::endl;
                            return { { tokenVector[rightBraceIndex].line, tokenVector[rightBraceIndex].index }, tokenVector[rightBraceIndex].content }; 
                        }

                        start = rightBraceIndex + 1;
                    }
                    // skip else  (if there is one)
                    if (tokenVector[start].type == TokenType::ELSE)
                    {
                        // find where { is
                        // the last else must be "else", not else if
                        if (tokenVector[start+1].type != TokenType::LEFT_BRACE)
                        {
                            return { { tokenVector[start+1].line, tokenVector[start+1].index }, tokenVector[start+1].content };  
                        }
                        leftBraceIndex = start + 1;
                        // find }
                        rightBraceIndex = findRightBraceNoError(tokenVector, leftBraceIndex + 1, rightBound);
                        // ERROR: } not found
                        if (rightBraceIndex > rightBound)
                        { return { { tokenVector[rightBraceIndex].line, tokenVector[rightBraceIndex].index }, tokenVector[rightBraceIndex].content }; }
                        
                        start = rightBraceIndex + 1;
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

            while (tokenVector[start].type != TokenType::SEMICOLON){
                start += 1;
            }

            std::pair<std::pair<int, int>, std::string> errorResult;            
            if (printIndex + 1 <= start-1 && tokenVector[printIndex+1].type == TokenType::LEFT_BRACKET) 
            {
                node->content2 = std::make_unique<ArrayNode>(tokenVector[printIndex+1]);
                errorResult = HandleArray(tokenVector, printIndex + 1, start -1, node->content2);
                
            }
            else 
            {
                errorResult = MakeExpressionTree(tokenVector, printIndex + 1, start - 1, node->content);
            }

            if (errorResult.first.first != -1) 
            {
                return errorResult;
            }
            nodes.push_back(std::move(node));
        }
        else if (tokenVector[start].type == TokenType::RETURN)
        {
            std::unique_ptr<ReturnNode> node = std::make_unique<ReturnNode>(tokenVector[start]);
            int returnIndex = start;
            start += 1;
            while (tokenVector[start].type !=TokenType::SEMICOLON)
            {
                start += 1;
            }
            // there can be nothing following "return"
            if (returnIndex + 1 <= start - 1)
            {
                auto errorResult = MakeExpressionTree(tokenVector, returnIndex + 1, start - 1, node->content);
                if (errorResult.first.first != -1) 
                {
                    return errorResult;
                }
            }
            else 
            {
                node->content = nullptr;
            }
            nodes.push_back(std::move(node));
        }
        // only array without any assignment
        else if (tokenVector[start].type == TokenType::LEFT_BRACKET) 
        {
            int EqualityIndex = -1;
            int beginIndex = start;
            int AssignmentIndex = -1;
            start += 1;
            while (tokenVector[start].line == tokenVector[beginIndex].line && start <= rightBound)
            {
                if (tokenVector[start].content ==  "==" || tokenVector[start].content == "!=" ) {
                    if (tokenVector[start-1].content != "]"){
                        start += 1;
                        continue;
                    }
                    EqualityIndex = start;
                }
                if (tokenVector[start].content ==  "=") {
                    AssignmentIndex = start;
                }
                start += 1;
            }
            if (AssignmentIndex != -1) 
            {
                std::unique_ptr<ExpressionNode> node = std::make_unique<ExpressionNode>(tokenVector[AssignmentIndex]);
                std::unique_ptr<ArrayNode> node1 = std::make_unique<ArrayNode>();
                // std::cout << "r: " << AssignmentIndex << beginIndex << std::endl;
                auto errorResult1 = HandleArray(tokenVector, beginIndex, AssignmentIndex-1, node1);
                if (errorResult1.first.first != -1) 
                {
                    return errorResult1;
                }
                node1->value.type = TokenType::LEFT_BRACKET;
                node->children2.push_back(std::move(node1));
    
                if (tokenVector[AssignmentIndex+1].type != TokenType::LEFT_BRACKET) {
                    std::unique_ptr<ExpressionNode> node2 = std::make_unique<ExpressionNode>();
                    auto errorResult2 = MakeExpressionTree(tokenVector, AssignmentIndex+1, start-1, node2);
                    if (errorResult1.first.first != -1) 
                    {
                        return errorResult1;
                    }
                    node->children2.push_back(std::move(node2));
                }
                else {
                    std::unique_ptr<ArrayNode> node2 = std::make_unique<ArrayNode>();
                    auto errorResult2 = HandleArray(tokenVector, AssignmentIndex+1, start-1, node2);
                    if (errorResult1.first.first != -1) 
                    {
                        return errorResult1;
                    }
                    node2->value.type = TokenType::ARRAY;
                    node->children2.push_back(std::move(node2));
                }
    
                nodes.push_back(std::move(node));
                return { { -1, -1 }, "" };
            }
            // std::cout << "t" <<std::endl;
            std::unique_ptr<ArrayNode> node = std::make_unique<ArrayNode>(tokenVector[beginIndex]);
            auto errorResult = HandleArray(tokenVector, beginIndex, start - 1, node, EqualityIndex); 
            if (errorResult.first.first != -1) 
            {
                return errorResult;
            }     
            if (EqualityIndex!=-1){
                node->EqualityIndex = EqualityIndex;
            }
            nodes.push_back(std::move(node));
        }
        // len()
        // else if (tokenVector[start].content == "len" && tokenVector[start+1].type == TokenType::LEFT_PARENTHESIS)
        // {
        //     start += 1;
        //     // Case1: when the parameter is an array ex. len([1,2,3]), len([])
        //     if (tokenVector[start+1].type== TokenType::LEFT_BRACKET) {

        //     }
        //     std::cout << "yes" << std::endl;
            
        // }
        else
        {
            // std::cout << "running1" << std::endl;
            bool arrayExist = false; // distinguished whether it is array   
            int beginIndex = start;
            start += 1;
            int EqualityIndex = -1;
    
            while (tokenVector[start].line == tokenVector[beginIndex].line && start <= rightBound)
            {
                if (tokenVector[start].type == TokenType::LEFT_BRACKET){
                    if (tokenVector[start-1].type == TokenType::TRUE || tokenVector[start-1].type == TokenType::FALSE){
                        tokenVector[start-1].type = TokenType::VARIABLE;
                        // variableTypeMap[tokenVector[start-1].content] = DataType::UNINITIALIZED;
                        start+= 1; 
                        continue;
                    }
                    if (tokenVector[start-1].type == TokenType::LEFT_PARENTHESIS) {
                        start += 1;
                        continue;
                    }
                    if (tokenVector[start-1].type != TokenType::VARIABLE ){     //check whether it is creating a new array
                        arrayExist = true;
                    }
                }


                if (tokenVector[start].content ==  "==" || tokenVector[start].content == "!=" ) {
                    // std::cout << "yes" << std::endl;
                    EqualityIndex = start;
                }
                start += 1;
            }

            if (arrayExist) 
            {
                tokenVector[beginIndex].type = TokenType::ARRAY;
                std::unique_ptr<ArrayNode> node = std::make_unique<ArrayNode>(tokenVector[beginIndex]);
                auto errorResult = HandleArray(tokenVector, beginIndex, start - 1, node, EqualityIndex); 
                if (EqualityIndex!=-1){
                    node->EqualityIndex = EqualityIndex;
                }
                // std::cout << "yes1" << std::endl;
                if (errorResult.first.first != -1) 
                {
                    return errorResult;
                }     
                nodes.push_back(std::move(node));          
            }
            else {
                std::unique_ptr<ExpressionNode> node = std::make_unique<ExpressionNode>(tokenVector[beginIndex]);
                auto errorResult = MakeExpressionTree(tokenVector, beginIndex, start - 1, node);
                if (errorResult.first.first != -1) 
                {
                    return errorResult;
                }
                nodes.push_back(std::move(node));
            } 
        }
    }
    return { { -1, -1 }, "" };
}

std::pair<std::pair<int, int>, std::string> ParserB::HandleArray(std::vector<Token> tokenVector, int leftBound, int rightBound, std::unique_ptr<ArrayNode>& node,  int index)
{
    if (leftBound > rightBound) {
        std::cout << "ERROR1" << std::endl;
    }
    int leftBracket = leftBound;

    // error check
    while (leftBracket < rightBound)
    {

        if (tokenVector[leftBracket].type == TokenType::LEFT_BRACKET) { break; }
        leftBracket += 1;
    }
    // [ not found
    if (leftBracket== rightBound)
    {
        return { { tokenVector[leftBracket].line, tokenVector[leftBracket].index }, tokenVector[leftBracket].content };
    }
    // find ]
    int rightBracket = ParserB::findRightBraceNoError(tokenVector, leftBracket + 1, rightBound);
    

    if (rightBracket > rightBound)
    {
        std::cout << "2" << std::endl;
        return { { tokenVector[rightBracket].line, tokenVector[rightBracket].index }, tokenVector[rightBracket].content };  
    }
    if (rightBracket + 1 <= rightBound && tokenVector[rightBracket + 1].type== TokenType::LEFT_BRACKET) {
        node->lookUp = true;
        if (rightBracket + 3 <= rightBound) {
            if (tokenVector[rightBracket+3].type == TokenType::RIGHT_BRACKET) {
                if (tokenVector[rightBracket + 2].type == TokenType::NUMBER)
                {
                    node->lookUpIndex = tokenVector[rightBracket + 2].value;
                }
                else 
                {
                    node->lookUpStr = tokenVector[rightBracket + 2].content;
                }
            }
            else {
                // std::cout << "Ru" <<std::endl;

                std::unique_ptr<ExpressionNode> cur =  std::make_unique<ExpressionNode>();
                auto error = MakeExpressionTree(tokenVector, rightBracket+2, rightBound-1, cur);
                node->LookUpNode.push_back(std::move(cur));
            }
        }
        else {
            std::cout << "ERROR4" <<  std::endl;
            // node = std::make_unique<ExpressionNode>(expression[topIndex]);
            // int rightB = findRightBracketNoError( expression, topIndex+2, rightBound);
            // std::unique_ptr<ExpressionNode> cur =  std::make_unique<ExpressionNode>();
            // auto error = MakeExpressionTree(expression, topIndex+2, rightB-1, cur);
            // node->children.push_back(std::move(cur));
            // node->ArrayLookUp = true;
            // node->value.type = TokenType::ARRAY;
        }
        // return { { -1, -1 }, "" };
    }

    if (index!=-1)
    {
        node = std::make_unique<ArrayNode>(tokenVector[index]);

        // on the left
        if (tokenVector[index-1].type == TokenType::RIGHT_BRACKET) 
        {
            std::unique_ptr<ArrayNode> node1 = std::make_unique<ArrayNode>();
            std::pair<std::pair<int, int>, std::string> errorResult1 = HandleArray(tokenVector, leftBound, index-1, node1);
            if (errorResult1.first.first != -1) { return errorResult1; }
            node1->value.type = TokenType::ARRAY;
            node->ArrayContent.push_back(std::move(node1));
        }
        else 
        {
            std::unique_ptr<ArrayNode> node1 = std::make_unique<ArrayNode>(tokenVector[index-1]);
            node1->value.type = TokenType::VARIABLE;
            node->ArrayContent.push_back(std::move(node1));
        }

        // on the right
        if (tokenVector[index+1].type == TokenType::LEFT_BRACKET) 
        {
            std::unique_ptr<ArrayNode> node2 = std::make_unique<ArrayNode>();
            std::pair<std::pair<int, int>, std::string> errorResult2 = HandleArray(tokenVector, index+1, rightBound, node2);
            if (errorResult2.first.first != -1) { return errorResult2; }
            node2->value.type = TokenType::ARRAY;
            node->ArrayContent.push_back(std::move(node2));
        }
        else 
        {
            std::unique_ptr<ArrayNode> node2 = std::make_unique<ArrayNode>(tokenVector[index+1]);
            node2->value.type = TokenType::VARIABLE;
            node->ArrayContent.push_back(std::move(node2));
        }

        return { { -1, -1 }, "" };
    }

    for (int index = leftBracket + 1; index < rightBracket; index++) {
        int commaIndex = index;
        while (commaIndex < rightBracket) { 

            if (tokenVector[commaIndex].type == TokenType::COMMA){
                break;
            }
            commaIndex += 1;
        }
        // Case 1: when the element is single token ex. true, 1
        if (commaIndex == index + 1) {
            std::unique_ptr<ExpressionNode> curr = std::make_unique<ExpressionNode>(tokenVector[index]);
            node->ArrayContent.push_back(std::move(curr));
        }
        // Case 2: when the element is an array ex. [1,2]
        else if (tokenVector[index].type == TokenType::LEFT_BRACKET) {  
            std::unique_ptr<ArrayNode> curr = std::make_unique<ArrayNode>();
            auto errorResult = HandleArray(tokenVector, index , commaIndex-1 , curr);
            curr->value.type = TokenType::ARRAY;
            if (errorResult.first.first != -1) 
            {
                return errorResult;
            }
            node->ArrayContent.push_back(std::move(curr));
        }
        // Case 3: when the element is an expression ex. 1 + 1, 2*3
        else {
            std::unique_ptr<ExpressionNode> curr;
            auto errorResult = ParserB::MakeExpressionTree(tokenVector, index , commaIndex-1 , curr);
            if (errorResult.first.first != -1) 
            {
                return errorResult;
            }
            node->ArrayContent.push_back(std::move(curr));
        }
        index = commaIndex;
    }
    return { { -1, -1 }, "" };
}

std::pair<std::pair<int, int>, std::string> ParserB::MakeExpressionTree(std::vector<Token> expression, int leftBound, int rightBound, std::unique_ptr<ExpressionNode>& node)
{ 

    if (expression[rightBound].type == TokenType::SEMICOLON)
    {
        rightBound -= 1;
    }

    if (leftBound > rightBound) {
#if DEBUG
    std::cout << "1  no expression  " << std::endl;
#endif
        return { { expression[leftBound].line, expression[leftBound].index }, expression[leftBound].content };
    }
    
    // Find the top level token
    int topIndex = leftBound;
    int i = leftBound ;
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
            { topIndex = topIndex < i ? topIndex : i; }
            // left associative
            else
            { topIndex = topIndex > i ? topIndex : i; }
        }
        // lower hierarchy
        else
        { // do nothing 
        }

        // Find the next element
        // ignore content inside (...)
        if (expression[i].type == TokenType::LEFT_PARENTHESIS || expression[i].type == TokenType::LEFT_BRACKET)
        {
            int rightIndex = findRightParenthesisNoError(expression, i+1, rightBound);
            if (rightIndex > rightBound)
            {
                return { { expression[rightIndex].line, expression[rightIndex].index }, expression[rightIndex].content };  
            }
            i = rightIndex + 1;
        }
        // function call
        else if (i+1 <= rightBound && expression[i].type == TokenType::VARIABLE && expression[i+1].type == TokenType::LEFT_PARENTHESIS)
        {
            int rightIndex = findRightParenthesisNoError(expression, i+2, rightBound);
            if (rightIndex > rightBound)
            {
                return { { expression[rightIndex].line, expression[rightIndex].index }, expression[rightIndex].content };  
            }
            i = rightIndex + 1;
        }
        // array
        else if (i+1 <= rightBound && expression[i].type == TokenType::VARIABLE && expression[i+1].type == TokenType::LEFT_BRACKET)
        {
            int rightIndex = findRightBracketNoError(expression, i+2, rightBound);
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

    // case 0 null
    if (expression[topIndex].type == TokenType::NUL)
    {
        node = std::make_unique<ExpressionNode>(expression[topIndex]);
        return { { -1, -1 }, "" };
    }
    // case 1 number
    else if (expression[topIndex].type == TokenType::NUMBER)
    {
        node = std::make_unique<ExpressionNode>(expression[topIndex]);
        return { { -1, -1 }, "" };
    }
    // case 2 variable
    else if (expression[topIndex].type == TokenType::VARIABLE)
    {
        // function call
        // recognize a function call if variable followed by (...)      e.g. add(1, 2)
        // Seperate a function call into function and parameters.   e.g. "getFunc()(1,2)(3)" should be seperated as "getFunc()(1,2)" and "(3)", 
        if (topIndex+1 <= rightBound && expression[topIndex+1].type == TokenType::LEFT_PARENTHESIS)
        {
            // function array member function len(), pop(), push()
            if (expression[topIndex].content == "len" || expression[topIndex].content == "pop" || expression[topIndex].content == "push" )
            {
                if (expression[topIndex].content == "len" ) 
                {
                    node = std::make_unique<ExpressionNode>(expression[topIndex]);
                    int leftParen = topIndex+1;
                    int rightParen = findRightParenthesisNoError(expression, leftParen+1, rightBound);
                    std::vector<std::unique_ptr<Node>> Flows;
                    auto errorMessage = ParserB::HandleTokenVector(expression, leftParen+1, rightParen-1, Flows);
                    // std::cout << "running" <<std::endl;
                    if (errorMessage.first.first != -1) {
                        return errorMessage;
                    }
                    node->children2.push_back(std::move(Flows[0]));   
                }
            }     
            else {
                node = std::make_unique<ExpressionNode>(expression[topIndex]);
                // getFunc()(1,2)()
                // get The last pair of parenthesis  (...)
                int leftParenthesisIndex = topIndex + 1;
                int rightParenthesisIndex;
                while (true) {
                    rightParenthesisIndex = findRightParenthesisNoError(expression, leftParenthesisIndex + 1, rightBound);
                    if (rightParenthesisIndex > rightBound)
                    {
                        return { { expression[rightParenthesisIndex].line, expression[rightParenthesisIndex].index }, expression[rightParenthesisIndex].content };  
                    }
                    if (rightParenthesisIndex + 1 <= rightBound && expression[rightParenthesisIndex + 1].type == TokenType::LEFT_PARENTHESIS)
                    {
                        leftParenthesisIndex = rightParenthesisIndex + 1;
                    }
                    else {
                        break;
                    }
                }


                // Function
                std::unique_ptr<ExpressionNode> functionNode = std::make_unique<ExpressionNode>();
                MakeExpressionTree(expression, topIndex, leftParenthesisIndex - 1, functionNode);
                node->children.push_back(std::move(functionNode));

                // Parameters
                // add parameter values into children
                // parameter values are seperated by comma. Remember to skip parenthesis.
                int left = leftParenthesisIndex + 1;

                while (left <= rightParenthesisIndex-1)
                {
                    std::unique_ptr<ExpressionNode> parameterNode = std::make_unique<ExpressionNode>();
                    int right = left;
                    while (right <= rightParenthesisIndex-1 && expression[right].type != TokenType::COMMA)
                    {
                        // Skip parenthesis  e.g. add(add(1, 2), 3)
                        if (expression[right].type == TokenType::LEFT_PARENTHESIS)
                        {
                            right = findRightParenthesisNoError(expression, right+1, rightParenthesisIndex-1);
                            if (right > rightParenthesisIndex-1)
                            {
                                return { { expression[right].line, expression[right].index }, expression[right].content };  
                            }
                        }
                        right += 1;
                    }

                    MakeExpressionTree(expression, left, right-1, parameterNode);
                    node->children.push_back(std::move(parameterNode));
                    left = right + 1;

                }
            }
            
        }
        // array lookup
        else if (topIndex+1 <= rightBound && expression[topIndex+1].type == TokenType::LEFT_BRACKET)
        {
            if (topIndex+3 <= rightBound && expression[topIndex+3].type == TokenType::RIGHT_BRACKET) { // make sure it is correct lookup format
                node = std::make_unique<ExpressionNode>(expression[topIndex]);
                if (expression[topIndex+2].type== TokenType::NUMBER){
                    node->index = expression[topIndex+2].value;
                }
                else {
                    // std::cout << "e: " <<  expression[topIndex+2].content<<std::endl;
                    node->lookUpStr = expression[topIndex+2].content;
                }
                node->ArrayLookUp = true;
            }
            else {
                node = std::make_unique<ExpressionNode>(expression[topIndex]);
                int rightB = findRightBracketNoError( expression, topIndex+2, rightBound);
                std::unique_ptr<ExpressionNode> cur =  std::make_unique<ExpressionNode>();
                auto error = MakeExpressionTree(expression, topIndex+2, rightB-1, cur);
                node->children.push_back(std::move(cur));
                node->ArrayLookUp = true;
                if (node->children[0]->value.type == TokenType::BIGGER_EQUAL ||node->children[0]->value.type == TokenType::SMALLER_EQUAL 
                || node->children[0]->value.type == TokenType::EQUALITY || node->children[0]->value.type == TokenType::INEQUALITY)
                {node->LookUpForm = false;}
                node->value.type = TokenType::VARIABLE;
                // if (node->value.type == TokenType::VARIABLE){
                //     std::cout << node->value.content << " " <<node->children[0]->value.content << std::endl;
                // }
            }
        }
        // variable
        else
        {
            node = std::make_unique<ExpressionNode>(expression[topIndex]);
        }
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
        // Handled in runtime
        
        // on the left
        std::unique_ptr<ExpressionNode> node1;
        // std::cout << "::" << expression[leftBound].content <<std::endl;
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
        std::pair<std::pair<int, int>, std::string> errorResult2;
        std::unique_ptr<ExpressionNode> node2;
        errorResult2 = MakeExpressionTree(expression, topIndex+1, rightBound, node2);
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
    // else if (expression[topIndex].type == TokenType::LEFT_BRACKET)
    // {
    //     std::unique_ptr<ArrayNode> curr = std::make_unique<ArrayNode>(expression[topIndex]);
    //     auto errorMessage = HandleArray(expression, leftBound)
    // }
    // case 8 ERROR
#if DEBUG
    std::cout << "3   " << std::endl;
#endif
    return { { expression[topIndex].line, expression[topIndex].index }, expression[topIndex].content };
}

std::string ParserB::calculate(Node* root, Result& result)
{
    auto& variableTypeMap = ScopeStack.top()->variableTypeMap;
    auto& variableArrayMap = ScopeStack.top()->variableArrayMap;

    // Def
    if (root->value.type == TokenType::DEF)
    {
        // std::cout << "DEFFFFFFFFF" << std::endl;
        FunctionDefineNode* functionDefineNode = dynamic_cast<FunctionDefineNode*>(root);
        Result result;
        result.type = DataType::FUNCTION;
        result.function = std::make_shared<Function>();
        result.function->m_ParameterNames = functionDefineNode->parameterNames;
        result.function->m_FunctionFlows = std::move(functionDefineNode->flows);
        setVariable(functionDefineNode->functionName, result);
        result.function->setScope(*ScopeStack.top());
        functionStorage.push_back(result.function);
        return "";
    }
    // While
    if (root->value.type == TokenType::WHILE)
    {
        WhileNode* whileNode = dynamic_cast<WhileNode*>(root);
        while (true)
        {
            // check condition
            Result conditionResult;
            std::string errorMessageCondition = calculate(whileNode->condition.get(), conditionResult);
            // ERROR: Condition is not a bool
            if (conditionResult.type != DataType::BOOL) { return "Runtime error: condition is not a bool."; }
            if (errorMessageCondition != "") { return errorMessageCondition; }
            if (conditionResult.boolValue == 0) { break; }
            // execute flows
            for (int i=0; i < (int)whileNode->flows.size(); i++)
            {
                std::string errorMessageFlow = calculate(whileNode->flows[i].get(), result);
                if (errorMessageFlow != "") { return errorMessageFlow; }  
                if (whileNode->flows[i].get()->value.type == TokenType::RETURN) { return ""; }          
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
            Result conditionResult;
            std::string errorMessageCondition = calculate(ifNode->conditions[conditionIndex].get(), conditionResult);
            // ERROR: Condition is not a bool
            if (conditionResult.type != DataType::BOOL) { return "Runtime error: condition is not a bool."; }
            if (errorMessageCondition != "") { return errorMessageCondition; }         
            if (conditionResult.boolValue == 1) 
            {
                // execute flows
                for (int i=0; i < (int)ifNode->flowGroups[conditionIndex].size(); i++)
                {
                    std::string errorMessageFlow = calculate(ifNode->flowGroups[conditionIndex][i].get(), result);
                    if (errorMessageFlow != "") { return errorMessageFlow; }
                    if (ifNode->flowGroups[conditionIndex][i].get()->value.type == TokenType::RETURN) { 
                        return ""; 
                    }
                }
                return "";
            }

        }

        // check else
        if (ifNode->conditions.size() != ifNode->flowGroups.size())
        {
            for (int i=0; i < (int)ifNode->flowGroups[ifNode->flowGroups.size()-1].size(); i++)
            {
                std::string errorMessageFlow = calculate(ifNode->flowGroups[ifNode->flowGroups.size()-1][i].get(), result);
                if (errorMessageFlow != "") { return errorMessageFlow; }  
                if (ifNode->flowGroups[ifNode->flowGroups.size()-1][i].get()->value.type == TokenType::RETURN) { return ""; }          
            }
            return "";
        }
        return "";
    }
    // Print
    else if (root->value.type == TokenType::PRINT)
    {
        PrintNode* printNode = dynamic_cast<PrintNode*>(root);
        Result flowResult;
        std::string errorMessageFlow; 
        if (printNode->content2 != nullptr) {
            errorMessageFlow = calculate(printNode->content2.get(), flowResult);
        }
        else {
            errorMessageFlow = calculate(printNode->content.get(), flowResult);
        }
        if (errorMessageFlow != "") { return errorMessageFlow; }           
        ParserB::printValue(flowResult);
        std::cout << std::endl;
    }
    // Return
    else if (root->value.type == TokenType::RETURN)
    {
        ReturnNode* returnNode = dynamic_cast<ReturnNode*>(root);
        if (returnNode->content != nullptr)
        {
            std::string errorMessageFlow = calculate(returnNode->content.get(), result);
            result.isreturn = true;
            if (errorMessageFlow != "") { return errorMessageFlow; }            
        } 
        else
        {
            returnNode->content = std::make_unique<ExpressionNode>(Token(TokenType::NUL, "null", -1, -1, -1));
            std::string errorMessageFlow = calculate(returnNode->content.get(), result);
            result.isreturn = true;
            if (errorMessageFlow != "") { return errorMessageFlow; }      
        }
    }
    // Equality
    else if (root->EqualityIndex != -1)
    {
        ArrayNode * Array = dynamic_cast<ArrayNode*>(root);
        if(Array->ArrayContent[0]->value.type == TokenType::VARIABLE)
        {
            if (variableTypeMap.at(Array->ArrayContent[0]->value.content) == DataType::ARRAY){
                if(variableArrayMap.at(Array->ArrayContent[0]->value.content)->ArrayContent.size() !=  Array->ArrayContent[0]->ArrayContent.size()){
                    result.type = DataType::BOOL;
                    result.boolValue = 0;
                }
            }
        }
        else 
        {
            if (variableTypeMap.at(Array->ArrayContent[1]->value.content) == DataType::ARRAY){
                if(variableArrayMap.at(Array->ArrayContent[1]->value.content)->ArrayContent.size() !=  Array->ArrayContent[1]->ArrayContent.size()){
                    result.type = DataType::BOOL;
                    result.boolValue = 0;
                }
            }
        }
        
        return "";
    }

    // Array
    else if (root->value.type == TokenType::ARRAY || root->value.type == TokenType::LEFT_BRACKET) // may cause memory leak check again !!!!
    {
        ArrayNode * Array = dynamic_cast<ArrayNode*>(root);
        result.type = DataType::ARRAY;
        result.arrayValue = std::make_shared<ArrayNode>(Array->value);
        result.arrayValue->lookUp = Array->lookUp;
        result.arrayValue->lookUpStr = Array->lookUpStr;
        result.arrayValue->lookUpIndex = Array->lookUpIndex;
        
        
        for (auto element : Array->ArrayContent){

            // std::cout << element->value.content << " " << element->value.value <<std::endl;
            if (element->value.type == TokenType::NUMBER)
            {
                result.arrayValue->ArrayContent.push_back(element);
            }

            else if (element->value.type == TokenType::FALSE || element->value.type == TokenType::TRUE)\
            {
                result.arrayValue->ArrayContent.push_back(element);
            }

            else if (element->value.type == TokenType::LEFT_BRACKET)
            {
                Result arrayResult;
                std::string errorMessage;
                errorMessage = calculate (element.get(), arrayResult);
                if (errorMessage != "") { return errorMessage; } 
                result.arrayValue->ArrayContent.push_back(element);

            }
            else if (element->value.type == TokenType::NUL)
            {
                result.arrayValue->ArrayContent.push_back(element);
            }
            else if (element->value.type == TokenType::ARRAY)
            {
                Result arrayResult;
                std::string errorMessage;
                errorMessage = calculate (element.get(), arrayResult);
                element->subArray = true;
                result.arrayValue->ArrayContent.push_back(element);
            }
            else if (element->value.type == TokenType::VARIABLE)
            {
                Result arrayResult;
                std::string errorMessage;
                errorMessage = calculate (element.get(), arrayResult);
                if (arrayResult.type == DataType::DOUBLE) {
                    element->value.value = arrayResult.doubleValue;
                    element->value.type = TokenType::NUMBER;
                    result.arrayValue->ArrayContent.push_back(element);
                }
                else if (arrayResult.type == DataType::NUL){
                    
                    element->value.type = TokenType::NUL;
                    result.arrayValue->ArrayContent.push_back(element);
                }
                else if (arrayResult.type == DataType::BOOL){
                    element->value.type = TokenType::TRUE;
                    element->value.value = arrayResult.boolValue;
                }
                else if (arrayResult.type == DataType::ARRAY){
                    element->value.type = TokenType::ARRAY;
                    element = arrayResult.arrayValue;
                    result.arrayValue->ArrayContent.push_back(element);
                }
            }
            // + - * / % ...
            else 
            {
                Result NumberResult;
                std::string errorMessage;
                calculate (element.get(), NumberResult);
                if (errorMessage != "") {return errorMessage;} 
                element->value.value = NumberResult.doubleValue;
                element->value.type = TokenType::NUMBER;
                
                result.arrayValue->ArrayContent.push_back(element);
            }
        }
        if (Array->lookUp == true) 
        {
            // x[1+2] x[1>=2]
            if (Array->lookUpStr == "" && Array->lookUpIndex == -1 && Array->LookUpNode.size()==0) 
            {   
                Result lookupResult;
                calculate(Array->ArrayContent[0].get(), lookupResult);
                if (lookupResult.type == DataType::DOUBLE) {Array->lookUpIndex = lookupResult.doubleValue; }
                else {return "Runtime error: index is not a number.";}
            }
            if (Array->lookUpStr != "" || Array->LookUpNode.size()!=0){ return "Runtime error: index is not a number."; }
            double a,b;
            b = std::modf(Array->lookUpIndex,&a);

            if (b!= 0) { return "Runtime error: index is not an integer."; }
            // if (variableTypeMap.at(Array->value.content) != DataType::ARRAY) { return "Runtime error: not an array."; }
            int I = result.arrayValue->ArrayContent.size();
            if (Array->lookUpIndex< 0 || Array->lookUpIndex >= I ) { return "Runtime error: index out of bounds."; }

            if (result.arrayValue->ArrayContent[Array->lookUpIndex]->value.type == TokenType::NUMBER) 
            {
                result.doubleValue = result.arrayValue->ArrayContent[Array->lookUpIndex]->value.value;
                result.type = DataType::DOUBLE;
            }
            else if (result.arrayValue->ArrayContent[Array->lookUpIndex]->value.type == TokenType::TRUE ||
                    result.arrayValue->ArrayContent[Array->lookUpIndex]->value.type == TokenType::FALSE)
            {   
                result.boolValue = result.arrayValue->ArrayContent[Array->lookUpIndex]->value.value;
                result.type = DataType::BOOL;    
            }
            else 
            {
                result.arrayValue = result.arrayValue->ArrayContent[Array->lookUpIndex];
                result.type = DataType::ARRAY;
            }
            
        }
        else {
            result.type = DataType::ARRAY;
            setVariable(Array->value.content, result);
        }
    }

    // Expression
    else 
    {
        ExpressionNode* expressionNode = dynamic_cast<ExpressionNode*>(root);

        // null
        if (expressionNode->value.type == TokenType::NUL)
        {
            result.type = DataType::NUL;

            return "";
        }

        // number
        else if (expressionNode->value.type == TokenType::NUMBER)
        {
            result.type = DataType::DOUBLE;
            result.doubleValue = expressionNode->value.value;

            return "";
        }

        // true
        else if (expressionNode->value.type == TokenType::TRUE)
        {
            result.type = DataType::BOOL;
            result.boolValue = expressionNode->value.value;

            return "";
        }

        // false
        else if (expressionNode->value.type == TokenType::FALSE)
        {
            result.type = DataType::BOOL;
            result.boolValue = expressionNode->value.value;

            return "";
        }

        // variable
        else if (expressionNode->value.type == TokenType::VARIABLE)
        {
            // Function Call
            if (expressionNode->children.size() != 0 && expressionNode->ArrayLookUp == false)
            {
                if (variableTypeMap.at(expressionNode->value.content) == DataType::UNINITIALIZED)
                {
                    return "Runtime error: unknown identifier " + expressionNode->value.content;
                }
                // variable is not a function
                if (variableTypeMap.at(expressionNode->value.content) != DataType::FUNCTION)
                {
                    return "Runtime error: not a function.";
                }

                // get the function stored in the first child
                Result functionResult;
                calculate(expressionNode->children[0].get(), functionResult);
                std::shared_ptr<Function> function = functionResult.function;
                
                // Parameters don't match
                if (function->m_ParameterNames.size() != expressionNode->children.size()-1)
                {
                    return "Runtime error: incorrect argument count.";
                }

                // execute the function
                // 1. create a new scope
                Scope* localScope = new Scope(*ScopeStack.top());
                // Overwrite the localscope with the captured scope
                localScope->OverwriteBy(*(function->m_CaptureScope));

                ScopeStack.push(localScope); 
                // 2. set parameter values
                for (int i = 1; i < (int)expressionNode->children.size(); i++)
                {
                    Result parameterResult;
                    calculate(expressionNode->children[i].get(), parameterResult);
                    setVariable(function->m_ParameterNames[i-1], parameterResult);
                }
                // 3. Execute flows
                for (int i=0; i < (int)function->m_FunctionFlows.size(); i++)
                {
                    calculate(function->m_FunctionFlows[i].get(), result);
                    if (result.isreturn)
                    {
                        ScopeStack.pop();
                        delete localScope;
                        return "";
                    }
                }
                result.type = DataType::NUL;
                ScopeStack.pop();
                delete localScope;
            }
            else if (expressionNode->children2.size() != 0 && expressionNode->ArrayLookUp == false)
            {
                result.type = DataType::DOUBLE;
                if (expressionNode->children2[0]->value.type == TokenType::LEFT_BRACKET)
                {
                    result.doubleValue = expressionNode->children2[0]->ArrayContent.size();   
                }
                else if (expressionNode->children2[0]->value.type == TokenType::VARIABLE)
                {
                    if (variableTypeMap.at(expressionNode->children2[0]->value.content) != DataType::ARRAY) { return "Runtime error: not an array.";}
                    Result result1;
                    getVariable (expressionNode->children2[0]->value.content, result1);
                    result.doubleValue = result1.arrayValue->ArrayContent.size();
                }
                return "";
            }
            // uninitialized
            if (variableTypeMap.find(expressionNode->value.content) == variableTypeMap.end() && expressionNode->value.content == "true"){
                 variableTypeMap[expressionNode->value.content] = DataType::UNINITIALIZED;
            }
            
            // Array lookup
            if (expressionNode->ArrayLookUp == true)
            {
                if (variableTypeMap.at(expressionNode->value.content) != DataType::ARRAY) { return "Runtime error: not an array."; }

                if (expressionNode->LookUpForm == false) {
                    return "Runtime error: index is not a number.";
                }

                if (variableTypeMap.at(expressionNode->value.content) == DataType::UNINITIALIZED)
                {
                    return "Runtime error: unknown identifier " + expressionNode->value.content;
                }

                
                // x[1+2] x[1>=2]
                if (expressionNode->lookUpStr == "" && expressionNode->index == -1) 
                {   
                    Result lookupResult;
                    calculate(expressionNode->children[0].get(), lookupResult);
                    if (lookupResult.type == DataType::DOUBLE) {expressionNode->index = lookupResult.doubleValue; }
                    else {return "Runtime error: index is not a number.";}
                }
                double a,b;
                if (expressionNode->lookUpStr != "")
                { 
                    if (variableTypeMap.at(expressionNode->lookUpStr) == DataType::UNINITIALIZED)
                    {
                        return "Runtime error: index is not a number."; 
                    }
                    getVariable (expressionNode->lookUpStr, result);
                    if (result.type !=DataType::DOUBLE) { return "Runtime error: index is not a number.";}
                    double a,b;
                    b = std::modf(result.doubleValue,&a);
                    if (b!= 0) { return "Runtime error: index is not an integer."; }
                    return ""; 
                }
                b = std::modf(expressionNode->index,&a);

                if (b!= 0) { return "Runtime error: index is not an integer."; }
        
                int I = variableArrayMap.at(expressionNode->value.content)->ArrayContent.size();
                if (expressionNode->index < 0 || expressionNode->index >= I ) { return "Runtime error: index out of bounds."; }

                getVariable(expressionNode->value.content, result, expressionNode->index);
                // std::cout << "running: " << expressionNode->index<<" "<< result.doubleValue << std::endl;
            }
            // Array
            else if (variableTypeMap.at(expressionNode->value.content) == DataType::ARRAY && expressionNode->ArrayLookUp == false) 
            {   
                if (variableTypeMap.at(expressionNode->value.content) == DataType::UNINITIALIZED)
                {
                    return "Runtime error: unknown identifier " + expressionNode->value.content;
                }
                getVariable(expressionNode->value.content, result);
            }
            else 
            {
                if (variableTypeMap.at(expressionNode->value.content) == DataType::UNINITIALIZED)
                {
                    return "Runtime error: unknown identifier " + expressionNode->value.content;
                }
                getVariable(expressionNode->value.content, result);
            }
            return "";
        }

        // =
        else if (expressionNode->value.type == TokenType::ASSIGNMENT)
        {
            
            if (expressionNode->children2.size()!= 0) 
            {
                std::string errorMessage = calculate(expressionNode->children2[1].get(), result);
                if (errorMessage != "") { return errorMessage; }
                return "";
            }

            // the last child must be a number or an initialized variable
            std::string errorMessage = calculate(expressionNode->children[1].get(), result);
            if (errorMessage != "") { return errorMessage; }

            // the first child must be a variable
            if (expressionNode->children[0].get()->value.type != TokenType::VARIABLE)
            { return "Runtime error: invalid assignee."; }
            if (expressionNode->children[0]->LookUpForm == false) {return "Runtime error: index is not a number.";}
            Result result2;
            if (expressionNode->children[0]->ArrayLookUp == true) 
            {
                // std::cout << "running"  << expressionNode->value.content <<std::endl;
                errorMessage = calculate(expressionNode->children[0].get(), result2);
                if (errorMessage != "") { return errorMessage; }
            }
            if (expressionNode->children[0]->index!= -1) {setVariable(expressionNode->children[0]->value.content, result, expressionNode->children[0]->index);}
            else {setVariable(expressionNode->children[0]->value.content, result);}

            

            return "";
        }

        // + - * / % == != > < >= <= & | ^
        else {
            // variable for operation is uninitialaized
            Result result1;
            Result result2;
            
            std::string errorMessage1 = calculate(expressionNode->children[0].get(), result1);
            if (errorMessage1 != "") { return errorMessage1; }
            std::string errorMessage2 = calculate(expressionNode->children[1].get(), result2);
            if (errorMessage2 != "") { return errorMessage2; }


            // handle type errors and set resultType
            // arithematic operations
            // + - * / %
            if (expressionNode->value.type == TokenType::PLUS || expressionNode->value.type == TokenType::MINUS || expressionNode->value.type == TokenType::MULTIPLY || 
                expressionNode->value.type == TokenType::DIVIDE || expressionNode->value.type == TokenType::MOD) 
            {   
                // arithmetic operator only works on double and return double
                if (result1.type == DataType::BOOL || result2.type == DataType::BOOL)
                {
                    return "Runtime error: invalid operand type.";
                }
                result.type = DataType::DOUBLE;
                // + - * /
                if (expressionNode->value.type == TokenType::PLUS) {
                    result.doubleValue = result1.doubleValue + result2.doubleValue; 
                }
                else if (expressionNode->value.type == TokenType::MINUS) {
                    result.doubleValue = result1.doubleValue - result2.doubleValue; 
                    // std::cout << "Here: " << result.doubleValue << std::endl;
                }
                else if (expressionNode->value.type == TokenType::MULTIPLY) { 
                    result.doubleValue = result1.doubleValue * result2.doubleValue; 
                }
                else if (expressionNode->value.type == TokenType::DIVIDE)
                {
                    if (result2.doubleValue == 0) {
                        return "Runtime error: division by zero.";
                    }
                    result.doubleValue = result1.doubleValue / result2.doubleValue; 
                }
                // %
                else if (expressionNode->value.type == TokenType::MOD) {
                    result.doubleValue = std::fmod(result1.doubleValue, result2.doubleValue); 
                }
            }
            // ordered comparison
            // < <= > >= 
            else if (expressionNode->value.type == TokenType::SMALLER || expressionNode->value.type == TokenType::SMALLER_EQUAL || 
                    expressionNode->value.type == TokenType::BIGGER  || expressionNode->value.type == TokenType::BIGGER_EQUAL) 
            { 
                // Comparsion operator only compare double and return bool
                if (result1.type == DataType::BOOL || result2.type == DataType::BOOL)
                {
                    return "Runtime error: invalid operand type.";
                }
                result.type = DataType::BOOL;
                // >
                if (expressionNode->value.type == TokenType::BIGGER) {
                    result.boolValue = result1.doubleValue > result2.doubleValue; 
                }
                // <
                else if (expressionNode->value.type == TokenType::SMALLER) {
                    result.boolValue = result1.doubleValue < result2.doubleValue; 
                }
                // >=
                else if (expressionNode->value.type == TokenType::BIGGER_EQUAL) {
                    result.boolValue = result1.doubleValue >= result2.doubleValue; 
                }
                // <=
                else if (expressionNode->value.type == TokenType::SMALLER_EQUAL) {
                    result.boolValue = result1.doubleValue <= result2.doubleValue; 
                }
            }
            // logic operations
            // & | ^
            else if (expressionNode->value.type == TokenType::AND || expressionNode->value.type == TokenType::INCLUSIVE_OR || 
                    expressionNode->value.type == TokenType::EXCLUSIVE_OR) 
            {
                if (result1.type != DataType::BOOL || result2.type != DataType::BOOL)
                {
                    return "Runtime error: invalid operand type.";
                }
                result.type = DataType::BOOL;
                // &
                if (expressionNode->value.type == TokenType::AND) {
                    result.boolValue = result1.boolValue && result2.boolValue; 
                }
                // |
                else if (expressionNode->value.type == TokenType::INCLUSIVE_OR) {
                    result.boolValue = result1.boolValue || result2.boolValue; 
                }
                // ^
                else if (expressionNode->value.type == TokenType::EXCLUSIVE_OR) {
                    result.boolValue = result1.boolValue != result2.boolValue;
                }
            }
            // Equality and Inequality
            // == !=
            else if (expressionNode->value.type == TokenType::EQUALITY || expressionNode->value.type == TokenType::INEQUALITY)
            {
                result.type = DataType::BOOL;
                // ==
                if (expressionNode->value.type == TokenType::EQUALITY) {

                    if (result1.type == DataType::NUL && result2.type == DataType::NUL)
                    {
                        result.boolValue = true;
                    }
                    else if (result1.type != result2.type)
                    {
                        result.boolValue = false;
                    }
                    else if (result1.type == DataType::ARRAY && result2.type == DataType::ARRAY)
                    {
                        if (result1.arrayValue->ArrayContent.size()==result2.arrayValue->ArrayContent.size()){
                            for (size_t i = 0; i < result1.arrayValue->ArrayContent.size(); i++){
                                // if (result1.arrayValue->ArrayContent.at(i)->value.type != result2.arrayValue->ArrayContent.at(i)->value.type 
                                // ||  result1.arrayValue->ArrayContent.at(i)->value.value != result2.arrayValue->ArrayContent.at(i)->value.value)
                                // {
                                //     result.boolValue = false;
                                //     return "";
                                // }
                                if (result1.arrayValue->ArrayContent.at(i)->value.type== TokenType::NUMBER) {
                                    if (result1.arrayValue->ArrayContent.at(i)->value.value != result2.arrayValue->ArrayContent.at(i)->value.value){
                                        result.boolValue = false;
                                        return "";
                                    }
                                }
                                if (result1.arrayValue->ArrayContent.at(i)->value.type== TokenType::TRUE || result1.arrayValue->ArrayContent.at(i)->value.type== TokenType::FALSE){
                                    if (result1.arrayValue->ArrayContent.at(i)->value.value != result2.arrayValue->ArrayContent.at(i)->value.value){
                                        result.boolValue = false;
                                        return "";
                                    }
                                }
                            }
                            result.boolValue = true;
                        }
                        else {result.boolValue = false;}
                    }
                    else
                    {
                        if (result1.type == DataType::DOUBLE)
                        { result.boolValue = result1.doubleValue == result2.doubleValue; }
                        else if (result1.type == DataType::BOOL)
                        { result.boolValue = result1.boolValue == result2.boolValue; }
                        else 
                        { std::cout << "Something's wrong == comparison"; }
                    }
                }
                // !=
                else if (expressionNode->value.type == TokenType::INEQUALITY) {
                    if (result1.type == DataType::NUL && result2.type == DataType::NUL)
                    {
                        result.boolValue = false;
                    }
                    else if (result1.type != result2.type)
                    {
                        result.boolValue = true;
                    }
                    else if (result1.type == DataType::ARRAY && result2.type == DataType::ARRAY)
                    {
                        if (result1.arrayValue->ArrayContent.size()==result2.arrayValue->ArrayContent.size()){
                            for (size_t i = 0; i < result1.arrayValue->ArrayContent.size(); i++){
                                if (result1.arrayValue->ArrayContent.at(i)->value.type == result2.arrayValue->ArrayContent.at(i)->value.type 
                                &&  result1.arrayValue->ArrayContent.at(i)->value.value == result2.arrayValue->ArrayContent.at(i)->value.value)
                                {
                                    result.boolValue = false;
                                    return "";
                                }
                            }
                            result.boolValue = true;
                        }
                        else {result.boolValue = true;}
                    }
                    else
                    {
                        if (result1.type == DataType::DOUBLE)
                        { result.boolValue = result1.doubleValue != result2.doubleValue; }
                        else if (result1.type == DataType::BOOL)
                        { result.boolValue = result1.boolValue != result2.boolValue; }
                        else 
                        { std::cout << "Something's wrong == comparison"; }
                    }
                }
            }

            return "";
        }
    }
    return "";
}


void ParserB::print(Node* root, int indent, bool semicolon)
{
    for (int i=0; i<indent; i++) { std::cout << "    "; }
    // Def
    if (root->value.type == TokenType::DEF)
    {
        FunctionDefineNode* functionDefineRoot = dynamic_cast<FunctionDefineNode*>(root);

        std::cout << "def ";
        std::cout << functionDefineRoot->functionName;
        std::cout << "(";
        for (int i = 0; i < (int)functionDefineRoot->parameterNames.size(); i++)
        {
            std::cout << functionDefineRoot->parameterNames[i];
            if (i != (int)functionDefineRoot->parameterNames.size()-1)
            {
                std::cout << ", ";
            }
        }
        std::cout << ") ";
        std::cout << "{" << std::endl;
        for (int i=0; i < (int)functionDefineRoot->flows.size(); i++)
        {
            print(functionDefineRoot->flows[i].get(), indent + 1);
            std::cout << std::endl;
        }

        for (int i=0; i < indent; i++) { std::cout << "    "; }
        std::cout << "}";
    }
    // While
    else if (root->value.type == TokenType::WHILE)
    {
        WhileNode* whileRoot = dynamic_cast<WhileNode*>(root);

        std::cout << "while ";
        print(whileRoot->condition.get(), 0, false);
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
        print(ifRoot->conditions[0].get(), 0, false);
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
            print(ifRoot->conditions[conditionIndex].get(), 0, false);
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
        if (printRoot->content2 != nullptr) {
            print(printRoot->content2.get(), 0, false);
        }
        else {
            print(printRoot->content.get(), 0, false);
        }
        std::cout << ";";
    }
    // Return
    else if (root->value.type == TokenType::RETURN)
    {
        ReturnNode* returnRoot = dynamic_cast<ReturnNode*>(root);

        std::cout << "return";
        if (returnRoot->content != nullptr) {
            std::cout << " ";
            print(returnRoot->content.get(), 0, false);
            std::cout << ";";
        }
        else {
            std::cout << ";";
        }
    }
    // Array
    else if (root->value.type == TokenType::ARRAY)
    {
        ArrayNode * ArrayRoot = dynamic_cast<ArrayNode*>(root);
        if ( ArrayRoot->value.content != "") {
            std::cout << "(" << ArrayRoot->value.content << " = ";
        }
        std::cout << "[";
        for(size_t i = 0; i < ArrayRoot->ArrayContent.size(); i++) {
            print(ArrayRoot->ArrayContent[i].get(), 0, false);
            if (i+1 < ArrayRoot->ArrayContent.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "]";
        if ( ArrayRoot->value.content != "") {
            std::cout << ");";
        }
    }
    // only Array without assignment
    else if (root->value.type == TokenType::LEFT_BRACKET && root->EqualityIndex == -1 ){
        // std::cout << "r" << std::endl;
        ArrayNode * ArrayRoot = dynamic_cast<ArrayNode*>(root);
        std::cout << "[";
        for(size_t i = 0; i < ArrayRoot->ArrayContent.size(); i++) {
            print(ArrayRoot->ArrayContent[i].get(), 0, false);
            if (i+1 < ArrayRoot->ArrayContent.size()) {
                std::cout << ", ";
            }
        }
        // check if there is lookup
        if (ArrayRoot->lookUp == true) 
        {
            if (ArrayRoot->LookUpNode.size()!=0) {
                // std::cout << "RUN" <<std::endl;
                std::cout << "][";
                print(ArrayRoot->LookUpNode.at(0).get(), 0 , false);
            }
            else if (ArrayRoot->lookUpIndex != -1) {
                std::cout << "][" << ArrayRoot->lookUpIndex;
            }
            else {
                std::cout << "][" << ArrayRoot->lookUpStr;
            }
        }
        if (semicolon == false) { std::cout << "]"; }
        else { std::cout << "];"; }
    }
    // array equality 
    else if (root->EqualityIndex != -1) 
    {
        ArrayNode * ArrayRoot = dynamic_cast<ArrayNode*>(root);
        std::cout << "(";
        if (ArrayRoot->ArrayContent[0]->ArrayContent.size()==0) {
            std::cout << ArrayRoot->ArrayContent[0]->value.content;
        }
        else 
        {
            print(ArrayRoot->ArrayContent[0].get());
        }
        std::cout <<" " <<ArrayRoot->value.content << " ";
        if (ArrayRoot->ArrayContent[1]->ArrayContent.size()==0) {

            std::cout << ArrayRoot->ArrayContent[1]->value.content;
        }
        else 
        {
            print(ArrayRoot->ArrayContent[1].get());
        }
        std::cout << ")";
        // std::cout << "yes" << std::endl;
    }
    // ExpressionNode
    else 
    {
        ExpressionNode* expressionNode = dynamic_cast<ExpressionNode*>(root);
        if (expressionNode->value.type == TokenType::NUMBER){

        }
        if (expressionNode->value.type == TokenType::NUL)
        {
            std::cout << "null";
        }
        else if (expressionNode->value.type == TokenType::NUMBER)
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
            // Function Call
            if (expressionNode->children.size() != 0 && expressionNode->ArrayLookUp == false)
            {
                print(expressionNode->children[0].get(), 0, false);
                std::cout << "(";
                for (int i = 1; i < (int)expressionNode->children.size(); i++)
                {
                    print(expressionNode->children[i].get(), 0, false);
                    if (i != (int)expressionNode->children.size() - 1) 
                    {
                        std::cout << ", ";
                    }
                }
                std::cout << ")";
                if (semicolon) { std::cout << ";"; }
            }
            // len() pop() push()
            else if (expressionNode->children2.size() != 0 && expressionNode->ArrayLookUp == false)
            {
                // len()
                if (expressionNode->value.content == "len") 
                {
                    std::cout << "len(";
                    if (expressionNode->children2[0]->value.type == TokenType::VARIABLE)
                    {
                        std::cout << expressionNode->children2[0]->value.content;
                    }
                    else 
                    {
                        print(expressionNode->children2[0].get(), 0, false);
                    }
                    
                    if (semicolon == true) {std::cout << ");";}
                    else                   {std::cout << ")";}
                }

            }
            // Array 
            else if (expressionNode->ArrayLookUp == true) {
                if (expressionNode->children.size()!= 0) {
                    std::cout << expressionNode->value.content << "[";
                    print(expressionNode->children[0].get(), 0, false);
                    std::cout << "]";
                }
                else if (expressionNode->index!=-1)
                {
                    std::cout << expressionNode->value.content << "[" << expressionNode->index << "]";
                }
                else if (expressionNode->lookUpStr != "") 
                {
                    std::cout << expressionNode->value.content << "[" << expressionNode->lookUpStr << "]";
                }
            }

            // Normal Variable
            else 
            {
                std::cout << expressionNode->value.content;
            }
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
                print(expressionNode->children[i].get(), 0, false);
                if (i != (int)expressionNode->children.size() - 1)
                {
                    std::cout << " " << expressionNode->value.content << " ";
                }
            }
            for (int i = 0; i < (int)expressionNode->children2.size(); i++)
            {
                print(expressionNode->children2[i].get(), 0, false);
                if (i != (int)expressionNode->children2.size() - 1)
                {
                    std::cout << " " << expressionNode->value.content << " ";
                }
            }
            std::cout << ")";
            if (semicolon) { std::cout << ";"; }
        }
    }
}

void ParserB::printValue(Result& value)
{

    // if (value.print == false) {return;}

    if (value.type == DataType::NUL)
    {
        std::cout << "null";
    }

    else if (value.type == DataType::BOOL)
    {
        if (value.boolValue == 0) { std::cout << "false"; }
        else                      { std::cout << "true"; }
    }

    else if (value.type == DataType::DOUBLE)
    {
        std::cout << value.doubleValue;
    }

    else if (value.type == DataType::ARRAY)
    {
        std::cout << "[";
        for (size_t i = 0; i < value.arrayValue->ArrayContent.size(); i++) 
        { 
            // std::cout << value.arrayValue->ArrayContent.size() <<std::endl;
            if (value.arrayValue->ArrayContent[i]->value.type == TokenType::NUMBER)
            {
            
                std::cout << value.arrayValue->ArrayContent[i]->value.value;
            }
            else if (value.arrayValue->ArrayContent[i]->value.type == TokenType::ARRAY)
            {   


                Result curr;
                curr.type = DataType::ARRAY;
    
                curr.arrayValue = value.arrayValue->ArrayContent.at(i);
                // curr.arrayValue->value.type = ;
                
                printValue(curr);
                // std::cout << "[5]";
            }
            else if (value.arrayValue->ArrayContent[i]->value.type == TokenType::NUL)
            {
                std::cout <<"null";
            }
            else{
                // std::cout << 3 << std::endl;
                std::cout << value.arrayValue->ArrayContent[i]->value.content;
            }
            if (i+1<value.arrayValue->ArrayContent.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "]";
    }

    else
    {
        std::cout << (value.type == DataType::FUNCTION) << std::endl;
        std::cout << (value.type == DataType::NUL) << std::endl;
        std::cout << (value.type == DataType::UNINITIALIZED) << std::endl;
        std::cout << "There is something wrong PrintValue" << std::endl;
    }
}

// exclude left parenthesis
int ParserB::findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound)
{
    int balance = 1;
    int p = leftBound;
    while (p <= rightBound) {
        if (expression[p].type == TokenType::LEFT_PARENTHESIS || expression[p].type == TokenType::LEFT_BRACKET) { balance += 1; }
        else if (expression[p].type == TokenType::RIGHT_PARENTHESIS || expression[p].type == TokenType::RIGHT_BRACKET) { balance -= 1; }

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

// exclude left brace 
// it also can find bracket no error
int ParserB::findRightBraceNoError(std::vector<Token> expression, int leftBound, int rightBound)
{
    int balance = 1;
    int p = leftBound;
    while (p <= rightBound) {
        if (expression[p].type == TokenType::LEFT_BRACE || expression[p].type == TokenType::LEFT_BRACKET) { balance += 1; }
        else if (expression[p].type == TokenType::RIGHT_BRACE || expression[p].type == TokenType::RIGHT_BRACKET) { balance -= 1; }

        if (balance == 0) { break; }
        p += 1;
    }
    return p;
}


// void ParserB::setupExpression(std::vector<Token>& expression, int leftBound, int rightBound, )
void ParserB::setupExpression(std::vector<Token>& expression)
{
    auto& variableTypeMap = ScopeStack.top()->variableTypeMap;
    
    for (int i = 0; i < (int)expression.size()-1; i++)
    {
        Token token = expression[i];
        // handle new variable
        if (variableTypeMap.find(token.content) == variableTypeMap.end() && token.type == TokenType::VARIABLE)
        {
            variableTypeMap[token.content] = DataType::UNINITIALIZED;
        }
    }
}

void ParserB::getVariable(std::string& variableName, Result& result, int index) {
    auto& variableTypeMap = ScopeStack.top()->variableTypeMap;
    auto& variableDoubleMap = ScopeStack.top()->variableDoubleMap;
    auto& variableBoolMap = ScopeStack.top()->variableBoolMap;
    auto& variableFunctionMap = ScopeStack.top()->variableFunctionMap;
    auto& variableArrayMap = ScopeStack.top()->variableArrayMap;
    result.type = variableTypeMap[variableName];

    if (index != -1)
    {
        if (variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::NUMBER)
        {
            result.type = DataType::DOUBLE;
            result.doubleValue =  variableArrayMap[variableName]->ArrayContent.at(index)->value.value;
        }
        else if (variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::TRUE || 
                variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::FALSE)
        {
            result.type = DataType::BOOL;
            result.boolValue = variableArrayMap[variableName]->ArrayContent.at(index)->value.value;
        }
        else if (variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::ARRAY || 
                variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::LEFT_BRACKET)
        {
            result.type = DataType::ARRAY;

            result.arrayValue = std::make_unique<ArrayNode>(variableArrayMap[variableName]->ArrayContent.at(index)->value);
            result.arrayValue->lookUp = variableArrayMap[variableName]->ArrayContent.at(index)->lookUp;
            result.arrayValue->lookUpStr = variableArrayMap[variableName]->ArrayContent.at(index)->lookUpStr;
            result.arrayValue->lookUpIndex = variableArrayMap[variableName]->ArrayContent.at(index)->lookUpIndex;
            for (auto element : variableArrayMap[variableName]->ArrayContent.at(index)->ArrayContent){
                result.arrayValue->ArrayContent.push_back(element);
            }
        }
        else if (variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::NUL)
        {
            result.type = DataType::NUL;
        }
        return;
    }

    if (variableTypeMap.at(variableName) == DataType::ARRAY)
    {
        result.arrayValue = variableArrayMap[variableName];
        return;
    }

    if (result.type == DataType::NUL)
    {
        
    }
    else if (result.type == DataType::DOUBLE)
    {
        result.doubleValue = variableDoubleMap[variableName];
    }
    else if (result.type == DataType::BOOL)
    {
        result.boolValue = variableBoolMap[variableName];
    }        
    else if (result.type == DataType::FUNCTION)
    {
        result.function = variableFunctionMap[variableName];
    }
    else if (result.type == DataType::ARRAY)
    {
        result.arrayValue = variableArrayMap[variableName];
    }
    else 
    {
        // std::cout << (result.type == DataType::NUL) << std::endl;
        // std::cout << (result.type == DataType::UNINITIALIZED) << std::endl;
        std::cout << "There is a problem getVariable" << std::endl;
    }
}

void ParserB::setVariable(std::string& variableName, Result& result, int index) {
    auto& variableTypeMap = ScopeStack.top()->variableTypeMap;
    auto& variableDoubleMap = ScopeStack.top()->variableDoubleMap;
    auto& variableBoolMap = ScopeStack.top()->variableBoolMap;
    auto& variableFunctionMap = ScopeStack.top()->variableFunctionMap;
    auto& variableArrayMap =  ScopeStack.top()->variableArrayMap;

    if (index != -1) {
        // if (variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::NUMBER)
        if (result.type == DataType::DOUBLE)
        {
            variableArrayMap[variableName]->ArrayContent.at(index)->value.type = TokenType::NUMBER;
            // result.type = DataType::DOUBLE;
            variableArrayMap[variableName]->ArrayContent.at(index)->value.value = result.doubleValue;
        }
        // else if (variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::TRUE || 
        //         variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::FALSE)
        else if (result.type == DataType::BOOL)
        {
            // result.type = DataType::BOOL;
            variableArrayMap[variableName]->ArrayContent.at(index)->value.type = TokenType::TRUE;
            variableArrayMap[variableName]->ArrayContent.at(index)->value.value = result.boolValue;
        }
        // else if (variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::ARRAY || 
        //         variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::LEFT_BRACKET)
        else if (result.type == DataType::ARRAY)
        {
            // result.type = DataType::ARRAY;

            // result.arrayValue = std::make_unique<ArrayNode>(variableArrayMap[variableName]->ArrayContent.at(index)->value);
            // result.arrayValue->lookUp = variableArrayMap[variableName]->ArrayContent.at(index)->lookUp;
            // result.arrayValue->lookUpStr = variableArrayMap[variableName]->ArrayContent.at(index)->lookUpStr;
            // result.arrayValue->lookUpIndex = variableArrayMap[variableName]->ArrayContent.at(index)->lookUpIndex;
            // for (auto element : variableArrayMap[variableName]->ArrayContent.at(index)->ArrayContent){
            //     result.arrayValue->ArrayContent.push_back(element);
            // }

            variableArrayMap[variableName]->ArrayContent.at(index) = result.arrayValue;

        }
        // else if (variableArrayMap[variableName]->ArrayContent.at(index)->value.type == TokenType::NUL)
        else if (result.type == DataType::NUL)
        {
            // result.type = DataType::NUL;
            variableArrayMap[variableName]->ArrayContent.at(index)->value.type = TokenType::NUL;

            variableArrayMap[variableName]->ArrayContent.at(index)->value.content = "null";
        }
        return;
    }

    variableTypeMap[variableName] = result.type;
    
    if (result.type == DataType::NUL)
    {

    }
    else if (result.type == DataType::DOUBLE)
    {
        variableDoubleMap[variableName] = result.doubleValue;
    }
    else if (result.type == DataType::BOOL)
    {
        variableBoolMap[variableName] = result.boolValue;
    }        
    else if (result.type == DataType::FUNCTION)
    {
        variableFunctionMap[variableName] = result.function;
    }
    else if (result.type == DataType::ARRAY)
    {   
        // std::cout << variableName<<" yes: " << result.arrayValue->value.content << std::endl;
        variableArrayMap[variableName] = result.arrayValue;
        //  if (variableName == "arref") {
        //     variableArrayMap[variableName]->ArrayContent.at(1)->value.value = -1;
        // }
    }
    else
    {
        std::cout << "There is a problem setVariable" << std::endl;
    }
}