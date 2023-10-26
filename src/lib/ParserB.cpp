#include "ParserB.h"
#include <stack>

// Initialize static member variables
// std::stack<int> jumptoStack = stack<int>();
int currentLine = 1;
bool skipLine = 1;
std::vector<std::vector<Token>> ParserB::expressionLines = std::vector<std::vector<Token>>();
std::map<std::string, double> ParserB::variableMap = std::map<std::string, double>();
std::map<std::string, bool> ParserB::variableInitializedMap = std::map<std::string, bool>();
std::map<TokenType, int> ParserB::hierarchyMap = {
    {TokenType::number ,          0}, // 1 2 3
    {TokenType::variable ,        0}, // a b c
    {TokenType::leftParenthesis , 1}, // ()
    {TokenType::multiply ,        2}, // *
    {TokenType::divide ,          2}, // /
    {TokenType::mod ,             2}, // %
    {TokenType::plus ,            3}, // +
    {TokenType::minus ,           3},  // -
    {TokenType::smaller ,         4}, // <
    {TokenType::bigger ,          4}, // >
    {TokenType::smaller_equal ,   4}, // <=
    {TokenType::bigger_equal ,    4}, // >=
    {TokenType::equality ,        5}, // ==
    {TokenType::inequality ,      5}, // !=
    {TokenType::AND ,             6}, // &
    {TokenType::exclusive_or ,    7},  // ^
    {TokenType::inclusive_or ,    8}, // |
    {TokenType::equals ,          9}  // =

};

std::pair<std::pair<int, int>, std::string> ParserB::MakeTreeInfix(std::vector<Token> expression, int leftBound, int rightBound, Node& node)
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
    if (expression[topIndex].type == TokenType::leftParenthesis)
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
            if (expression[i].type == TokenType::equals)
            {
                topIndex = topIndex < i ? topIndex : i;
            }
            // left associative
            else
            {
                topIndex = topIndex > i ? topIndex : i;
            }
        }

        // ignore content inside (...)
        if (expression[i].type == TokenType::leftParenthesis)
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
    if (expression[topIndex].type == TokenType::number)
    {
        node = Node(expression[topIndex]);
        return { { -1, -1 }, "" };
    }
    // case 2 variable
    else if (expression[topIndex].type == TokenType::variable)
    {
        node = Node(expression[topIndex]);
        return { { -1, -1 }, "" };
    }
    // case 3 =
    else if (expression[topIndex].type == TokenType::equals)
    {
        Node res = Node(expression[topIndex]);

        // check assignment ERROR
        // Error 1. Nothing before =   e.g =8
        if (topIndex == leftBound)
            return { { expression[topIndex].line, expression[topIndex].index }, expression[topIndex].content }; 
        // Error 2. Nothing after =   e.g a=
        if (topIndex == rightBound)
            return { { expression[topIndex+1].line, expression[topIndex+1].index }, expression[topIndex+1].content }; 
        // Error 3. What is before = is not a variable      e.g 1=1
        if (expression[topIndex-1].type != TokenType::variable)
            return { { expression[topIndex].line, expression[topIndex].index }, expression[topIndex].content };  


        Node node1;
        std::pair<std::pair<int, int>, std::string> errorResult1 = MakeTreeInfix(expression, leftBound, topIndex-1, node1);
        if (errorResult1.first.first != -1) { return errorResult1; }
        res.children.push_back(node1);

        Node node2;
        std::pair<std::pair<int, int>, std::string> errorResult2 = MakeTreeInfix(expression, topIndex+1, rightBound, node2);
        if (errorResult2.first.first != -1) { return errorResult2; }
        res.children.push_back(node2);

        node = res;
        return { { -1, -1 }, "" };
    }
    // case 4 + - * / % < > <= >= != == & ^ | 
    else if (expression[topIndex].type == TokenType::plus || expression[topIndex].type == TokenType::minus || 
            expression[topIndex].type == TokenType::multiply || expression[topIndex].type == TokenType::divide || expression[topIndex].type == TokenType::mod ||
            expression[topIndex].type == TokenType::smaller || expression[topIndex].type == TokenType::bigger|| expression[topIndex].type == TokenType::smaller_equal ||
            expression[topIndex].type == TokenType::bigger_equal || expression[topIndex].type == TokenType::inequality || expression[topIndex].type == TokenType::equality ||
            expression[topIndex].type == TokenType::AND || expression[topIndex].type == TokenType::exclusive_or || expression[topIndex].type == TokenType::inclusive_or)
    {
        Node res = Node(expression[topIndex]);

        Node node1;
        std::pair<std::pair<int, int>, std::string> errorResult1 = MakeTreeInfix(expression, leftBound, topIndex-1, node1);
        if (errorResult1.first.first != -1) { return errorResult1; }
        res.children.push_back(node1);

        Node node2;
        std::pair<std::pair<int, int>, std::string> errorResult2 = MakeTreeInfix(expression, topIndex+1, rightBound, node2);
        if (errorResult2.first.first != -1) { return errorResult2; }
        res.children.push_back(node2);
        
        node = res;
        return { { -1, -1 }, "" };
    }
    // case 5 (...)
    else if (expression[topIndex].type == TokenType::leftParenthesis)
    {
        int rightIndex = findRightParenthesisNoError(expression, topIndex+1, rightBound);
        if (rightIndex > rightBound)
        {
            return { { expression[rightIndex].line, expression[rightIndex].index }, expression[rightIndex].content };  
        }
        return MakeTreeInfix(expression, topIndex+1, rightIndex-1, node);
    }
    
    // case 6 ERROR
    else
    {
#if DEBUG
    std::cout << "3   " << std::endl;
#endif
        return { { expression[topIndex].line, expression[topIndex].index }, expression[topIndex].content };
    }
}


// static void parseLines(std::vector<std::string> expressionLines)
// {
//     // line count start from 1
//     while (currentLine < expressions.size() + 1)
//     {
//         expression = expressions[i-1];

//         // Lexer
//         std::vector<Token> TokenVector;
//         std::pair<int, int> errorPair = Token::GenTokenVector(expression, TokenVector);
//         if (errorPair.first != -1)
//         {
//             std::cout << "Syntax error on line " << lineCount << " column " << errorPair.second << "." << std::endl;
//             exit(1);
//         }


//         // ParserB
//         ParserB::setupExpressionInfix(TokenVector); // register variables

//         if (TokenVector[0].type == TokenType::while)
//         {
//             // Parse
//             Node root;
//             std::pair<std::pair<int, int>, std::string> errorResult = ParserB::MakeTreeInfix(TokenVector, 1, TokenVector.size() - 3, root);
//             if (errorResult.first.first != -1) 
//             {
//                 std::cout << "Unexpected token at line " << currentLine << " column " << errorResult.first.second << ": " << errorResult.second << std::endl;
//                 exit(2);
//             }

//             // Calculate
//             double result;
//             std::string errorMessage = ParserB::calculate(root, result);

//             if (errorMessage.length() != 0)
//             {
//                 std::cout << errorMessage << std::endl;
//                 exit(3);

//                 continue;
//             }

//             if (result != 0)
//             {
//                 jumptoStack.push(currentLine);
//             }
//             else {

//             }
//         } 
//         else if (TokenVector[0].type == TokenType::if)
//         {
//             // Parse
//             Node root;
//             std::pair<std::pair<int, int>, std::string> errorResult = ParserB::MakeTreeInfix(TokenVector, 1, TokenVector.size() - 3, root);
//             if (errorResult.first.first != -1) 
//             {
//                 std::cout << "Unexpected token at line " << currentLine << " column " << errorResult.first.second << ": " << errorResult.second << std::endl;
//                 exit(2);
//             }

//             // Calculate
//             double result;
//             std::string errorMessage = ParserB::calculate(root, result);

//             if (errorMessage.length() != 0)
//             {
//                 std::cout << errorMessage << std::endl;
//                 exit(3);

//                 continue;
//             }
//         }
//         else if (TokenVector[0].type == TokenType::print)
//         {
//             // Parse
//             Node root;
//             std::pair<std::pair<int, int>, std::string> errorResult = ParserB::MakeTreeInfix(TokenVector, 1, TokenVector.size() - 2, root);
//             if (errorResult.first.first != -1) 
//             {
//                 std::cout << "Unexpected token at line " << currentLine << " column " << errorResult.first.second << ": " << errorResult.second << std::endl;
//                 exit(2);
//             }

//             // Calculate
//             double result;
//             std::string errorMessage = ParserB::calculate(root, result);

//             if (errorMessage.length() != 0)
//             {
//                 std::cout << errorMessage << std::endl;
//                 exit(3);

//                 continue;
//             }

//             std::cout << result << std::endl;
//             currentLine += 1;
//         }
//         else if (TokenVector[0].type == TokenType::rightBracket)
//         {
//             // redundant }
//             if (jumptoStack.empty())
//             {
//                 std::cout << "Unexpected token at line " << currentLine << " column " << TokenVector[0].index << ": " << TokenVector[0].content << std::endl;
//                 exit(2);
//             }

//             currentLine = jumptoStack.top();
//             jumptoStack.pop();
//             currentLine += 1;
//         }
//         else
//         {
//             // ParserB
//             Node root;
//             std::pair<std::pair<int, int>, std::string> errorResult = ParserB::MakeTreeInfix(TokenVector, 0, TokenVector.size() - 2, root);

//             if (errorResult.first.first != -1) 
//             {
//                 std::cout << "Unexpected token at line " << errorResult.first.first << " column " << errorResult.first.second << ": " << errorResult.second << std::endl;
//                 continue;
//             }
//             ParserB::print(root);
//             std::cout << std::endl;

//             // Calculate
//             double result;
//             std::string errorMessage = ParserB::calculate(root, result);

//             if (errorMessage.length() != 0)
//             {
//                 std::cout << errorMessage << std::endl;
//                 exit(3);

//                 continue;
//             }

//             currentLine += 1;
//         }
//     }

// }


std::string ParserB::calculate(Node root, double& result)
{ 
    // number
    if (root.value.type == TokenType::number)
    {
        result = root.value.value;
        return "";
    }

    // variable
    else if (root.value.type == TokenType::variable || root.value.type == TokenType::bool_varibale)
    {
        if (variableInitializedMap.find(root.value.content) == variableInitializedMap.end())
        {
            return "Runtime error: unknown identifier " + root.value.content;
        }
        result = variableMap.at(root.value.content);
        if (root.value.type == TokenType::bool_varibale) {
            return "Bool";
        }
        return "";
    }

    // =
    else if (root.value.type == TokenType::equals)
    {
        // the last child must be a number or an initialized variable
        Node last = root.children[root.children.size()-1];
        if (last.value.type == TokenType::variable && variableInitializedMap.find(last.value.content) == variableInitializedMap.end())
        {
            return "Runtime error: unknown identifier " + last.value.content;
        }

        std::string errorMessage = calculate(last, result);
        if (errorMessage != "" && errorMessage != "Bool") { return errorMessage; }

        if(errorMessage == "Bool"){
            variableMap.at(root.children[0].value.content) = result;
            variableInitializedMap.at(root.children[0].value.content) = true;
            root.value.type = TokenType::bool_varibale;
            return "Bool";
        }

        // set values
        for (int i = 0; i < (int)root.children.size()-1; i++)
        {
            variableMap.at(root.children[i].value.content) = result;
            variableInitializedMap.at(root.children[i].value.content) = true;
        }

        return "";
    }

    // + - * / %
    else if (root.value.type == TokenType::plus || root.value.type == TokenType::minus || root.value.type == TokenType::multiply || root.value.type == TokenType::divide ||
            root.value.type == TokenType::mod)
    {
        // variable for operation is uninitialaized
        if (root.children[0].value.type == TokenType::variable && variableInitializedMap.at(root.children[0].value.content) == false)
        {
            return "Runtime error: unknown identifier " + root.children[0].value.content;
        }
        if (root.children[0].value.type == TokenType::TRUE || root.children[0].value.type == TokenType::FALSE)
        {
            //error cannot caculate the bool value 
            // std::cout << "Runtime error: invalid operand type." << std::endl;
        }

        std::string errorMessage = calculate(root.children[0], result);
        if (errorMessage != "") { return errorMessage; }
        if (errorMessage == "Bool") {return "Runtime error: invalid operand type.";}


        for (int i = 1; i < (int)root.children.size(); i++)
        {
            // variable for operation is uninitialaized
            if (root.children[i].value.type == TokenType::variable && variableInitializedMap.at(root.children[i].value.content) == false)
            {
                return "Runtime error: unknown identifier " + root.children[i].value.content;
            }
            else if (root.children[i].value.type == TokenType::TRUE || root.children[i].value.type == TokenType::FALSE){
                //error cannot caculate the bool value 
                return "Runtime error: invalid operand type.";
            }
            else if (root.value.type == TokenType::plus) {
                double r;
                std::string errorMessage = calculate(root.children[i], r);
                if (errorMessage != "" && errorMessage != "Bool") { return errorMessage; }
                if (errorMessage == "Bool") {return "Runtime error: invalid operand type.";}
                result += r; 
            }
            else if (root.value.type == TokenType::minus) {
                double r;
                std::string errorMessage = calculate(root.children[i], r);
                if (errorMessage != "" && errorMessage != "Bool" ) { return errorMessage; }
                if (errorMessage == "Bool") {return "Runtime error: invalid operand type.";}
                result -= r; 
            }
            else if (root.value.type == TokenType::multiply) { 
                double r;
                std::string errorMessage = calculate(root.children[i], r);
                if (errorMessage != "" && errorMessage != "Bool") { return errorMessage; }
                if (errorMessage == "Bool") {return "Runtime error: invalid operand type.";}
                result *= r; 
            }
            else if (root.value.type == TokenType::divide)
            {
                double r;
                std::string errorMessage = calculate(root.children[i], r);
                if (errorMessage != "" && errorMessage != "Bool") { return errorMessage; }
                if (errorMessage == "Bool") {return "Runtime error: invalid operand type.";}
                if (r == 0) {
                    return "Runtime error: division by zero.";
                }
                result /= r; 
            }
            else if (root.value.type == TokenType::mod)
            {
                double r;
                std::string errorMessage = calculate(root.children[i], r);
                if (errorMessage != "" && errorMessage != "Bool") { return errorMessage; }
                if (errorMessage == "Bool") {return "Runtime error: invalid operand type.";}
                result = std::fmod(result,r); 
            }
        }
        return "";
    }
    // == != < <= >= > & | ^
    else if (root.value.type == TokenType::equality || root.value.type == TokenType::inequality || root.value.type == TokenType::smaller || 
            root.value.type == TokenType::smaller_equal || root.value.type == TokenType::bigger || root.value.type == TokenType::bigger_equal || 
            root.value.type == TokenType::AND || root.value.type == TokenType::inclusive_or || root.value.type == TokenType::exclusive_or) 
    {      
        // variable 1 for operation is uninitialaized
        if (root.children[0].value.type == TokenType::variable && variableInitializedMap.at(root.children[0].value.content) == false)
            {
                return "Runtime error: unknown identifier " + root.children[0].value.content;
            }
        

        std::string errorMessage = calculate(root.children[0], result);
        if (errorMessage != "" && errorMessage != "Bool") { return errorMessage; }

        std::string Last_errorMessage = errorMessage;

        // variable 2 for operation is uninitialaized
        if (root.children[1].value.type == TokenType::variable && variableInitializedMap.at(root.children[1].value.content) == false)
            {
                return "Runtime error: unknown identifier " + root.children[1].value.content;
            }

        double r;
        errorMessage = calculate(root.children[1], r);

        //when errorMessage == "" means there is no problem 
        //when errorMessage == "Bool" means it is a bool expression
        if (errorMessage != "" && errorMessage != "Bool") { return errorMessage; }

        if (root.children[0].value.type == TokenType::bool_varibale && errorMessage != "Bool"){
            return "Runtime error: invalid operand type.";
        }

        if ( Last_errorMessage != errorMessage) {
            return  "Runtime error: invalid operand type." ;
        }

        else if (root.value.type == TokenType::equality)
            {
                if (result == r){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        else if (root.value.type == TokenType::inequality)
            {
                if (result != r){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        else if (root.value.type == TokenType::smaller)
            {
                if (result < r){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        else if (root.value.type == TokenType::smaller_equal)
            {
                if (result <= r){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        else if (root.value.type == TokenType::bigger)
            {
                if (result > r){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        else if (root.value.type == TokenType::bigger_equal)
            {
                if (result >= r){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        else if (root.value.type == TokenType::AND)
            {
                if (result == 1 && r == 1){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        else if (root.value.type == TokenType::inclusive_or)
            {
                if (result == 1 || r == 1){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        else if (root.value.type == TokenType::exclusive_or)
            {
                if ((result != r)){
                    result = 1;
                }
                else {
                    result = 0;
                }
            }
        return "Bool";
    }
    return "WRONG";
}


void ParserB::print(Node root)
{
    if (root.value.type == TokenType::number)
    {
        int v = floor(root.value.value);
        // 1000.000  ->  1000
        if ((double)v == root.value.value)
        {
            std::cout << v;
        }
        else
        {
            std::cout << root.value.content;
        }
    }
    else if (root.value.type == TokenType::variable)
    {
        std::cout << root.value.content;
    }
    else {
        std::cout << "(";
        for (int i = 0; i < (int)root.children.size(); i++)
        {
            print(root.children[i]);
            if (i != (int)root.children.size() - 1)
            {
                std::cout << " " << root.value.content << " ";
            }
        }
        std::cout << ")";
    }
}

// exclude left parenthesis
int ParserB::findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound)
{
    int balance = 1;
    int p = leftBound;
    while (p <= rightBound) {
        if (expression[p].type == TokenType::leftParenthesis) { balance += 1; }
        if (expression[p].type == TokenType::rightParenthesis) { balance -= 1; }
        if (balance == 0) { break; }
        p += 1;
    }
    return p;
}

// exclude left bracket
// int ParserB::findRightBracketNoError(std::vector<Token> expression, int leftBound, int rightBound)
// {
//     int balance = 1;
//     int p = leftBound;
//     while (p <= rightBound) {
//         if (expression[p].type == TokenType::leftParenthesis) { balance += 1; }
//         if (expression[p].type == TokenType::rightParenthesis) { balance -= 1; }
//         if (balance == 0) { break; }
//         p += 1;
//     }
//     return p;
// }


// return vectors of lines in infix notation
// register variables in map 
void ParserB::setupExpressionInfix(std::vector<Token> expression)
{
    if (expression.size() == 1)
    {
#if DEBUG
    std::cout << "-1 empty expression" << std::endl;
#endif

        std::cout << "Unexpected token at line " << 1 << " column " << 1 << ": " << "END" << std::endl;
        exit(2);
    }

    for (int i = 0; i < (int)expression.size()-1; i++)
    {
        Token token = expression[i];
        // handle new variable
        if (token.type == TokenType::variable)
        {
            if (variableMap.find(token.content) == variableMap.end())
            {
                variableMap.insert({ token.content, -1 });
                variableInitializedMap.insert({ token.content, false });
            }
        }
    }
}



// std::string ParserB::Checkerror(Node root, bool &result){  // only check whether all variable have value or will it be assigned value in the expression
//     for (size_t i =0; i<root.children.size();i++){
//         if(root.children.at(i).value.type==TokenType::variable && root.value.type != TokenType::equals){
//             if (variableInitializedMap.at(root.children.at(i).value.content) == false){
//                 result = false;
//                 return root.children.at(i).value.content;
//             }
//         }
//     }
//     if(root.children.size()==0){
//         result = true;
//         return "";
//     }

//     if (Checkerror(root.children.at(0),result) != ""){
//         result = false;
//         return Checkerror(root.children.at(0), result);
//     }
//     else if (Checkerror(root.children.at(1), result) != ""){
//         result = false;
//         return Checkerror(root.children.at(0), result);
//     }  
//     result = true;
//     return "";
// }