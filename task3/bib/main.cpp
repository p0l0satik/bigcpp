#include <iostream>
#include "functions.hpp"

int main(int argc, char* argv[]){
    FunFactory fact;
    auto c2 = fact.CreateFunction("constant", { 0});
    auto c1 = fact.CreateFunction("constant", { 1.0});
    auto i3= fact.CreateFunction("identical", { 1.0, -1.0});
    auto i1= fact.CreateFunction("identical", { 3.0, -0});
    auto i0= fact.CreateFunction("identical", { 3.0, 1.0});
    auto s3 = fact.CreateFunction("sedate", { 3.0});
    auto e3 = fact.CreateFunction("exp", {10});
    auto p = fact.CreateFunction("polynomial", {-1, 0, 1, 3});
    auto p2 = fact.CreateFunction("polynomial", {7, 0, 3, 15});
    // std::cout << (*c2)(2.0) << std::endl;    
    // std::cout << (*i1)(0) << std::endl;    
    // std::cout << (*s3)(2.0) << std::endl; 
    std::cout << (*e3)(2) << std::endl;    
    // std::cout << (*p)(1.0) << std::endl;  
    // std::cout << i3->ToString() <<std::endl;
    // std::cout << i1->ToString() <<std::endl;
    // std::cout << i0->ToString() <<std::endl;
    // std::cout << c2->ToString() <<std::endl;
    // std::cout << s3->ToString() <<std::endl;
    std::cout << e3->ToString() <<std::endl;
    // std::cout << p->ToString() <<std::endl;

    auto c = e3 / i3;
    // auto k = *c + *(*c1 + *c2);
    p2 = p2*p2;
    auto p3 = fact.CreateFunction("polynomial", {1, 0, -3, 0, 1});
    auto f1 = fact.CreateFunction("polynomial", {1});
    auto f2 = fact.CreateFunction("polynomial", {0, 0, 1});
    auto f3 = fact.CreateFunction("polynomial", {-1, 0, 0, 1});
    std::cout <<  f3->ToString()<<  " " << f2->ToString() << " " << f3->ToString() <<std::endl;
    // std::cout <<  p3->GetDeriv(2) <<std::endl;
    double x = equation(f3 + f1 + f2, 0.0001, 10000000, 1);
    std::cout << x <<std::endl;
}