// #include <string>
// #include <iostream>
// #include <vector>
#include "lib/Parser.h"
#include "lib/lex.h"

int main() {

    std::cout << "Hi!" << std::endl;
    // std::vector<std::string> test;

//Case Test 1 : (+ 1 (* 2 3 4))
    // test.push_back("(");
    // test.push_back("+");
    // test.push_back("1");
    // test.push_back("(");
    // test.push_back("*");
    // test.push_back("2");
    // test.push_back("(");
    // test.push_back("*");
    // test.push_back("3");
    // test.push_back("4");
    // test.push_back(")");
    // test.push_back(")");
    // test.push_back(")");


// Case Test 2 : (* (+ 1 2) 3 (/ 4 5 (- 6 7)))
    // test.push_back("(");
    // test.push_back("*");
    // test.push_back("(");
    // test.push_back("+");
    // test.push_back("1");
    // test.push_back("2");
    // test.push_back(")");
    // test.push_back("3");
    // test.push_back("(");
    // test.push_back("/");
    // test.push_back("4");
    // test.push_back("5");
    // test.push_back("(");
    // test.push_back("-");
    // test.push_back("6");
    // test.push_back("7");
    // test.push_back(")");
    // test.push_back(")");
    // test.push_back(")");

// Case Test 3 : (/(+ 1 2) (- 3 4) (* 5 6))
    // test.push_back("(");
    // test.push_back("/");
    // test.push_back("(");
    // test.push_back("+");
    // test.push_back("1");
    // test.push_back("2");
    // test.push_back(")");
    // test.push_back("(");
    // test.push_back("-");
    // test.push_back("3");
    // test.push_back("4");
    // test.push_back(")");
    // test.push_back("(");
    // test.push_back("*");
    // test.push_back("5");
    // test.push_back("6");
    // test.push_back(")");
    // test.push_back(")");



    // Parser Tree(test);


    // if (Tree.Root == nullptr){
    //     std::cout << "wrong" << std::endl;
    // }
    // std::vector<Node *> ch = Tree.Root->Children.at(2)->Children;


    // std::vector<Node *> ch = Tree.Root->Children.at(2)->Children.at(2)->Children;

//     std::cout <<"Root: " << Tree.Root->token << std::endl;
//    // Print Vector
//     for (auto str : test){
//         std::cout << str << " ";
//     }
//     std::cout << std::endl;


//     for (auto & c: ch){
//         std::cout << c->token << std::endl;
//     }
    // std::cout <<"Root: " << Tree.Root->token << std::endl;

 


    return 0;
}