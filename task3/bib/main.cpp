#include <iostream>
#include "functions.hpp"

int main(int argc, char* argv[]){
    FunFactory fact;
    auto c2 = fact.CreateFunction("constant", std::vector<double> { 2.0});
    auto i3= fact.CreateFunction("identical", std::vector<double> { 3.0, -1.0});
    auto i1= fact.CreateFunction("identical", std::vector<double> { 3.0, -.0});
    auto i0= fact.CreateFunction("identical", std::vector<double> { 3.0, 1.0});
    auto s3 = fact.CreateFunction("sedate", std::vector<double> { 3.0});
    auto e3 = fact.CreateFunction("exp", std::vector<double> {10});
    auto p = fact.CreateFunction("polynomial", std::vector<double> {1, -2, 3});
    std::cout << (*c2)(2.0) << std::endl;    
    std::cout << (*i3)(2.0) << std::endl;    
    std::cout << (*s3)(2.0) << std::endl; 
    std::cout << (*e3)(1.0) << std::endl;    
    std::cout << (*p)(1.0) << std::endl;  
    std::cout << i3->ToString() <<std::endl;
    std::cout << i1->ToString() <<std::endl;
    std::cout << i0->ToString() <<std::endl;
    std::cout << c2->ToString() <<std::endl;
    std::cout << s3->ToString() <<std::endl;
    std::cout << e3->ToString() <<std::endl;
    std::cout << p->ToString() <<std::endl;

}