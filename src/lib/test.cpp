#include <iostream>
#include <cmath>
#include "Lexer.h"

int main(){
    double a = 1;
    double b = 0;
    // bool a = true;
    // bool b = true;
    double result ;
    // result = a % b;
    // std::cout << result << std::endl;
    // result *= a;
    // result *= b;
    Token test(TokenType::variable,"x", 1, 1, -1);
    test.type = TokenType::bool_varibale;

   

    if (test.type == TokenType::bool_varibale){ std::cout << "yes" << std::endl;}

    // std::cout <<  result << std::endl;
}