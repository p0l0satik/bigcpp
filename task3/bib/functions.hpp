#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <cmath>


class TFunction{
public:
    virtual double operator()(double x) = 0;
    virtual std::string ToString() = 0;
    std::shared_ptr<TFunction> copy(){};
};

using TFunPtr = std::shared_ptr<TFunction>;


class Addition : public TFunction{
    TFunPtr Fr, Sc;
public:
    Addition(TFunPtr fr, TFunPtr sc): Fr(fr), Sc(sc){}
    double operator()(double x){
        return (*Fr)(x) + (*Sc)(x); 
    }

    std::string ToString(){
        return Fr->ToString() + Sc->ToString();
    }
    TFunPtr copy(){
        return std::make_shared<Addition>(Fr, Sc);
    }
};

class Constant : public TFunction{
    double koef;
public:

    friend TFunPtr operator + (TFunction &left, TFunction &right);
    Constant(std::vector<double>& koefs): koef(koefs[0]){}
    double operator()(double x){return koef;}
    std::string ToString(){return std::to_string(koef);}
    TFunPtr copy(){
        std::vector<double> args = {koef};
        return std::make_shared<Constant>(args);
    }
};


class Identical : public TFunction{
    double a, b;
public:
    Identical(std::vector<double>& koefs): a(koefs[0]), b(koefs[1]){}
    double operator()(double x){return a * x + b;}
    std::string ToString(){return std::to_string(a) + "*x" + (b != 0 ? (b > 0 ? "+" :"") +  std::to_string(b) : "");}
};

class Sedate : public TFunction{
    double koef;
public:
    Sedate(std::vector<double>& koefs): koef(koefs[0]){}
    double operator()(double x){return pow(x, koef);}
    std::string ToString(){return "x^" + std::to_string(koef);}
};

class Exp : public TFunction{
    double base;
public:
    Exp(std::vector<double>& koefs) : base(koefs[0]){}
    double operator()(double x){return pow(base, x);}
    std::string ToString(){return std::to_string(base) + "^x";}
};

class Polynomial : public TFunction{
    std::vector<double> koefs;
public:
    Polynomial(std::vector<double>& koefs) : koefs(koefs){}
    double operator()(double x){
        double p = 0, sum = 0;
        for (auto k : koefs){
            sum += k * pow(x, p);
            p++;
        }
        return sum;
    }
    std::string ToString(){
        int p = 0;
        std::string str = "";
        for (auto k : koefs){
            if (k == 0) continue;
            if ( p == 0) {
                str += k > 0 ? "" : "- ";
                str += std::to_string(abs(k)) + " ";
            } else {
                str += k > 0 ? "+ " : "- ";
                str += std::to_string(abs(k)) + "*x^" + std::to_string(p) + " ";
            }
            p++;
        }
        return str;
    }
};

class FunFactory {
    class FunImpl;
    std::unique_ptr<FunImpl> Impl;
public:
    FunFactory();
    ~FunFactory();
    std::shared_ptr<TFunction> CreateFunction(const std::string& name, std::vector<double> koefs) const;
    std::vector<std::string> GetAvailableFunctions() const;

};