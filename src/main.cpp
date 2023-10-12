#include <string>
#include <iostream>
#include <vector>
#include "lib/Parser.h"

int main() {

    std::cout << "Hi!" << std::endl;
    //(+ 1 (* 2 3 4))
    std::vector<std::string> test;
    test.push_back("(");
    test.push_back("+");
    test.push_back("1");
    test.push_back("(");
    test.push_back("*");
    test.push_back("2");
    test.push_back("3");
    test.push_back("4");
    test.push_back(")");
    test.push_back(")");

    Parser Tree(test);
    // if (Tree.Root == nullptr){
    //     std::cout << "wrong" << std::endl;
    // }
    std::cout << Tree.Root->Children.size() << std::endl;
    // for (auto str : test){
    //     std::cout << str;
    // }
    // std::cout << std::endl;
 

    return 0;
}