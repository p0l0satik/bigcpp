#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class TFunction{
public:
    virtual double operator()(double x) = 0;
};

class Constant : public TFunction{
    double coef;
public:
    Constant(double a = 2): coef(a){}
    double operator()(double x){return x * coef;}
};

class FunFactory {
    class FunImpl;
    std::unique_ptr<FunImpl> Impl;
public:
    FunFactory();
    ~FunFactory();
    std::unique_ptr<TFunction> CreateFunction(const std::string& name) const;
    std::vector<std::string> GetAvailableFunctions() const;

};