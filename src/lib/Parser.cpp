#include "Parser.h"

std::vector<std::vector<Token>> Parser::expressionLines = std::vector<std::vector<Token>>();
std::map<std::string, double> Parser::variableMap = std::map<std::string, double>();
std::map<std::string, bool> Parser::variableInitializedMap = std::map<std::string, bool>();

Node Parser::MakeTree(std::vector<Token> expression, int leftBound, int rightBound)
{
    if (expression[leftBound].type == TokenType::leftParenthesis)
    {
        // ( not followed by operation symbol
        if (expression[leftBound + 1].type != TokenType::plus && expression[leftBound + 1].type != TokenType::minus && expression[leftBound + 1].type != TokenType::multiply && expression[leftBound + 1].type != TokenType::divide && expression[leftBound + 1].type != TokenType::equals)
        {
            // std::cout << "1" << std::endl;
            std::cout << "Unexpected token at line " << expression[leftBound + 1].line << " column " << expression[leftBound + 1].index << ": " << expression[leftBound + 1].content << std::endl;
            exit(2);
        }
        // right parentheses ) not found
        int rightIndex = findRightParenthesis(expression, leftBound + 1, rightBound);
        if (rightIndex > rightBound)
        {
            // std::cout << "2" << std::endl;
            std::cout << "Unexpected token at line " << expression[rightIndex].line << " column " << expression[rightIndex].index << ": " << expression[rightIndex].content << std::endl;
            exit(2);
        }
        return MakeTree(expression, leftBound + 1, rightIndex - 1);
    }

    // number
    else if (expression[leftBound].type == TokenType::number)
    {
        // must be one number
        // 0 1 2
        if (leftBound != rightBound)
        {
            // std::cout << "3" << std::endl;
            std::cout << "Unexpected token at line " << expression[leftBound + 1].line << " column " << expression[leftBound + 1].index << ": " << expression[leftBound + 1].content << std::endl;
            exit(2);
        }
        return Node(expression[leftBound]);
    }

    // variable
    else if (expression[leftBound].type == TokenType::variable)
    {
        // must be one variable
        // a 1 2
        if (leftBound != rightBound)
        {
            // std::cout << "4" << std::endl;
            std::cout << "Unexpected token at line " << expression[leftBound + 1].line << " column " << expression[leftBound + 1].index << ": " << expression[leftBound + 1].content << std::endl;
            exit(2);
        }
        return Node(expression[leftBound]);
    }

    // operations = + - * /
    else if (expression[leftBound].type == TokenType::plus || expression[leftBound].type == TokenType::minus || expression[leftBound].type == TokenType::multiply || expression[leftBound].type == TokenType::divide || expression[leftBound].type == TokenType::equals)
    {

        // nothing follows the symbol
        if (leftBound == rightBound)
        {
            // std::cout << "5" << std::endl;
            std::cout << "Unexpected token at line " << expression[leftBound + 1].line << " column " << expression[leftBound + 1].index << ": " << expression[leftBound + 1].content << std::endl;
            exit(2);
        }

        // iterate through all the elements and add them as children
        Node res = Node(expression[leftBound]);
        int p = leftBound + 1;
        while (p <= rightBound)
        {
            // number 
            if (expression[p].type == TokenType::number)
            {
                // when =, the elements other than the last element shouldn't be a number
                // = 3 a 4 b       3 = a = 4 = b
                if (p != rightBound && expression[leftBound].type == TokenType::equals)
                {
                    // std::cout << "6" << std::endl;
                    std::cout << "Unexpected token at line " << expression[p].line << " column " << expression[p].index << ": " << expression[p].content << std::endl;
                    exit(2);
                }



                res.children.push_back(MakeTree(expression, p, p));
                p += 1;
            }
            // variable 
            else if (expression[p].type == TokenType::variable)
            {
                res.children.push_back(MakeTree(expression, p, p));
                p += 1;
            }
            // (
            else if (expression[p].type == TokenType::leftParenthesis)
            {
                // // ( not followed by operation symbol
                // if (expression[p + 1].type != TokenType::plus && expression[p + 1].type != TokenType::minus && expression[p + 1].type != TokenType::multiply && expression[p + 1].type != TokenType::divide)
                // {
                //     std::cout << "Unexpected token at line " << expression[p + 1].line << " column " << expression[p + 1].index << ": " << expression[p + 1].content << std::endl;
                //     exit(2);
                // }

                int rightIndex = findRightParenthesis(expression, p + 1, rightBound);
                // // ) not found
                // if (rightIndex > rightBound)
                // {
                //     std::cout << "Unexpected token at line " << expression[rightIndex].line << " column " << expression[rightIndex].index << ": " << expression[rightIndex].content << std::endl;
                //     exit(2);
                // }
                res.children.push_back(MakeTree(expression, p, rightIndex));
                p = rightIndex + 1;
            }

            // ErrorToken
            else
            {
                // std::cout << "7" << std::endl;
                std::cout << "Unexpected token at line " << expression[p].line << " column " << expression[p].index << ": " << expression[p].content << std::endl;
                exit(2);
            }
        }
        return res;
    }
    std::cout << "Something wrong..." << std::endl;
    exit(2);
    return Node();
}


double Parser::calculate(Node root)
{
    // number
    if (root.value.type == TokenType::number)
    {
        return root.value.value;
    }

    // variable
    else if (root.value.type == TokenType::variable)
    {
        if (variableInitializedMap.at(root.value.content) == false)
        {
            std::cout << "Runtime error: unknown identifier " << root.value.content << std::endl;
            exit(3);
        }
        return variableMap.at(root.value.content);
    }

    // =
    else if (root.value.type == TokenType::equals)
    {
        // the last child must be a number or a initialized variable
        Node last = root.children[root.children.size()-1];
        if (last.value.type == TokenType::variable && variableInitializedMap.at(last.value.content) == false)
        {
            std::cout << "Runtime error: unknown identifier " << last.value.content << std::endl;
            exit(3);
        }
        double res = calculate(last);

        // set values
        for (int i = 0; i < (int)root.children.size()-1; i++)
        {
            variableMap.at(root.children[i].value.content) = res;
            variableInitializedMap.at(root.children[i].value.content) = true;
        }

        return res;
    }

    // + - * /
    else {
        // variable for operation is uninitialaized
        if (root.children[0].value.type == TokenType::variable && variableInitializedMap.at(root.children[0].value.content) == false)
        {
            std::cout << "Runtime error: unknown identifier " << root.children[0].value.content << std::endl;
            exit(3);
        }
        double res = calculate(root.children[0]);
        for (int i = 1; i < (int)root.children.size(); i++)
        {
            // variable for operation is uninitialaized
            if (root.children[i].value.type == TokenType::variable && variableInitializedMap.at(root.children[i].value.content) == false)
            {
                std::cout << "Runtime error: unknown identifier " << root.children[i].value.content << std::endl;
                exit(3);
            }
            if (root.value.type == TokenType::plus) { res += calculate(root.children[i]); }
            if (root.value.type == TokenType::minus) { res -= calculate(root.children[i]); }
            if (root.value.type == TokenType::multiply) { res *= calculate(root.children[i]); }
            if (root.value.type == TokenType::divide)
            {
                if (calculate(root.children[i]) == 0) {
                    std::cout << "Runtime error: division by zero." << std::endl;
                    exit(3);
                }
                res /= calculate(root.children[i]);
            }
        }
        return res;
    }
}


void Parser::print(Node root)
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

// exclude rightParenthesis
int Parser::findLeftParenthesis(std::vector<Token> expression, int leftBound, int rightBound)
{
    int balance = 1;
    int p = rightBound;
    while (p >= leftBound) {
        if (expression[p].type == TokenType::leftParenthesis) { balance -= 1; }
        if (expression[p].type == TokenType::rightParenthesis) { balance += 1; }
        if (balance == 0) { break; }
        p -= 1;
    }
    return p;
}

int Parser::findRightParenthesis(std::vector<Token> expression, int leftBound, int rightBound)
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


// return vectors of lines
// register variables in map 
void Parser::setupExpression(std::vector<Token> expression)
{
    std::vector<std::vector<Token>> res;
    std::vector<Token> current;
    int currentLine = 1;
    for (Token token : expression)
    {
        // handle new variable
        if (variableMap.find(token.content) == variableMap.end())
        {
            variableMap.insert({ token.content, -1 });
            variableInitializedMap.insert({ token.content, false });
        }

        // handle line
        if (token.line != currentLine)
        {
            while (currentLine != token.line)
            {
                current.push_back(expression[expression.size()-1]);
                res.push_back(current);
                current.clear();
                currentLine += 1;
            }
        }
        current.push_back(token);
    }
    res.push_back(current);
    expressionLines = res;
}
