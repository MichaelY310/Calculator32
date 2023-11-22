#include <cmath>
#include <iostream>

int main(){
    double a = 10.1;
    double c;
    double b = std::modf(a,&c);
    std::cout << b << std::endl;
}