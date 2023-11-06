#include <iostream>
#include <cmath>
// #include "Lexer.h"
#include <memory>

template <class T> 
void print(T a, T b){
    std::cout << "a is: " << a << ", b is: " << b << std::endl;
}

template <class T>
T add(T a, T b){
    return a+b;
}

template <class T>  class Animal {
    public:
    T name(T a){
        std::cout << a << std::endl;
        return a;
    }
};


struct Node {
    Node () { std::cout << "Constructor Running ..." << std::endl; }
    ~Node() { std::cout << "Deleting ..." << std::endl; }
};


int main(){
    
    print<int>(1,2);
    print<std::string>("Apple","Banana");

    std::string result = add<std::string>("Apple ","Banana");
    std::cout << result << std::endl;

    Animal<std::string> t;
    t.name("abv");
        // {
    //     // std::unique_ptr<Node> Test1 = std::make_unique<Node>();
    //     std::shared_ptr<Node> Test1 = std::make_shared<Node>();
    //     std::weak_ptr<Node> Test3(Test1);


    //     std::shared_ptr<Node> Test4 = Test3.lock();

    //     std::cout << "Use count : "<< Test3.use_count() << std::endl;
    //     std::shared_ptr<Node> Test2(Test1);
    //     std::cout << "Use count : " << Test2.use_count() << std::endl;
    //     Test1 = nullptr;
    //     std::cout << "Use count : " << Test2.use_count() << std::endl;
        
    // }
    // std::cout << "1" << std::endl;
    return 0;
    // std::cout <<  result << std::endl;
}