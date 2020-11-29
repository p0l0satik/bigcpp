#include <iostream>
#include "functions.hpp"

int main(int argc, char* argv[]){
    FunFactory fact;
    auto c2 = fact.CreateFunction("constant");
    std::cout << (*c2)(2.0) << std::endl;    
}