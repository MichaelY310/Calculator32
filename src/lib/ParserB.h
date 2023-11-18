#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <memory>
#include <stack>

#include "DataType.h"
#include "Lexer.h"
#include "Scope.h"
#include "Function.h"
#include "ASTNode.h"


class Result {
public:
    // Result(std::shared_ptr<ArrayNode> Array){

    // }
    DataType type;
    bool isreturn = false;
    std::shared_ptr<Function> function;
    std::shared_ptr<Node> arrayValue;
    union {
        double doubleValue;
        bool boolValue;
    };
};

class ParserB
{
public:
    static std::pair<std::pair<int, int>, std::string> HandleTokenVector(std::vector<Token> tokenVector, int leftBound, int rightBound, std::vector<std::unique_ptr<Node>>& nodes);
    static std::pair<std::pair<int, int>, std::string> MakeExpressionTree(std::vector<Token> expression, int leftBound, int rightBound, std::unique_ptr<ExpressionNode>& node);
    static std::pair<std::pair<int, int>, std::string> HandleArray(std::vector<Token> tokenVector, int leftBound, int rightBound,std::unique_ptr<ArrayNode>& node , int index = -1);
    static std::string calculate(Node* root, Result& result);
    static void print(Node* root, int indent = 0, bool semicolon = true);
    static void printValue(Result& result);
    static void setupExpression(std::vector<Token>& expression);
    static void getVariable(std::string& variableName, Result& result, int index = -1);
    static void setVariable(std::string& variableName, Result& result, int index = -1);
    static int findRightParenthesisNoError(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightBracketNoError(std::vector<Token> expression, int leftBound, int rightBound);
    static int findRightBraceNoError(std::vector<Token> expression, int leftBound, int rightBound);

    static std::stack<Scope*> ScopeStack;
    static std::map<TokenType, int> hierarchyMap;
    static std::vector<std::shared_ptr<Function>> functionStorage;

    static void clean()
    {
        for (int i=0; i < (int)functionStorage.size(); i++)
        {
            delete functionStorage[i]->m_CaptureScope;
        }
    }


    
static void print_no_semicolon(Node* root, int indent=0)
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
        print(printRoot->content.get(), 0, false);
    }
    // Return
    else if (root->value.type == TokenType::RETURN)
    {
        ReturnNode* returnRoot = dynamic_cast<ReturnNode*>(root);

        std::cout << "return ";
        print(returnRoot->content.get());
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
            std::cout << ")";
        }
    }
    // only Array without assignment
    else if (root->value.type == TokenType::LEFT_BRACKET && root->EqualityIndex == -1){
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
            if (ArrayRoot->lookUpIndex != -1) {
                std::cout << "][" << ArrayRoot->lookUpIndex;
            }
            else {
                std::cout << "][" << ArrayRoot->lookUpStr;
            }
        }
        std::cout << "]"; 
    }

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
            }
            else if (expressionNode->ArrayLookUp == true) {
                if (expressionNode->children.size()!= 0) {
                    std::cout << expressionNode->value.content << "[";
                    print(expressionNode->children[0].get(), 0, false);
                    std::cout << "]";
                }
                else
                {
                    std::cout << expressionNode->value.content << "[" << expressionNode->index << "]";
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
        }
    }
}
private:
};

