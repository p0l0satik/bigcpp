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
    virtual std::shared_ptr<TFunction> copy() = 0;
    virtual double GetDeriv(double x) = 0;
};

using TFunPtr = std::shared_ptr<TFunction>;


class Addition : public TFunction{
    char op;
    TFunPtr Fr, Sc;
public:
    Addition(TFunPtr fr, TFunPtr sc, char op): Fr(fr), Sc(sc), op(op){}
    double operator()(double x){
        if (op == '+')
            return (*Fr)(x) + (*Sc)(x); 
        else if (op == '-')
            return (*Fr)(x) - (*Sc)(x); 
        else if (op == '*')
            return (*Fr)(x) * (*Sc)(x); 
        else if (op == '/')
            return (*Fr)(x) / (*Sc)(x); 
        
        throw std::logic_error("impossible operation");
        return 0;
    }

    double GetDeriv(double x){
        if (op == '+')
            return Fr->GetDeriv(x) + Sc->GetDeriv(x); 
        else if (op == '-')
            return Fr->GetDeriv(x) - Sc->GetDeriv(x); 
        else if (op == '*')
            return Fr->GetDeriv(x) * (*Sc)(x) + Sc->GetDeriv(x) * (*Fr)(x); 
        else if (op == '/')
            return (Fr->GetDeriv(x) * (*Sc)(x) - Sc->GetDeriv(x) * (*Fr)(x)) / pow((*Sc)(x), 2); 
        throw std::logic_error("impossible operation");
        return 0;
    }

    std::string ToString(){
        return Fr->ToString() + " " + Sc->ToString();
    }
    TFunPtr copy(){
        return std::make_shared<Addition>(Fr, Sc, op);
    }
};

TFunPtr operator + (TFunPtr left, TFunPtr right);

template <typename T>
TFunPtr operator + (TFunPtr left, T right){ throw std::logic_error("right object is wrong"); return nullptr;};

template <typename T>
TFunPtr operator + (T left, TFunPtr right){throw std::logic_error("left object is wrong"); return nullptr;};

TFunPtr operator - (TFunPtr left, TFunPtr right);

template <typename T>
TFunPtr operator - (TFunPtr left, T right){ throw std::logic_error("right object is wrong"); return nullptr;};

template <typename T>
TFunPtr operator - (T left, TFunPtr right){throw std::logic_error("left object is wrong"); return nullptr;};

TFunPtr operator * (TFunPtr left, TFunPtr right);

template <typename T>
TFunPtr operator * (TFunPtr left, T right){ throw std::logic_error("right object is wrong"); return nullptr;};

template <typename T>
TFunPtr operator * (T left, TFunPtr right){throw std::logic_error("left object is wrong"); return nullptr;};

TFunPtr operator / (TFunPtr left, TFunPtr right);

template <typename T>
TFunPtr operator / (TFunPtr left, T right){ throw std::logic_error("right object is wrong"); return nullptr;};

template <typename T>
TFunPtr operator / (T left, TFunPtr right){throw std::logic_error("left object is wrong"); return nullptr;};

class Constant : public TFunction{
    double koef;
public:

    Constant(std::vector<double>& koefs): koef(koefs[0]){}
    double operator()(double x){return koef;}
    std::string ToString(){return std::to_string(koef);}
    TFunPtr copy(){
        std::vector<double> args = {koef};
        return std::make_shared<Constant>(args);
    }
    double GetDeriv(double x){
        return 0;
    }
};


class Identical : public TFunction{
    double a, b;
public:
    Identical(std::vector<double>& koefs): a(koefs[0]), b(koefs[1]){}
    double operator()(double x){return a * x + b;}
    std::string ToString(){return std::to_string(a) + "*x" + (b != 0 ? (b > 0 ? "+" :"") +  std::to_string(b) : "");}
    TFunPtr copy(){
        std::vector<double> args = {a, b};
        return std::make_shared<Identical>(args);
    }
    double GetDeriv(double x){
        return a;
    }
};

class Sedate : public TFunction{
    double koef;
public:
    Sedate(std::vector<double>& koefs): koef(koefs[0]){}
    double operator()(double x){return pow(x, koef);}
    std::string ToString(){return "x^" + std::to_string(koef);}
    TFunPtr copy(){
        std::vector<double> args = {koef};
        return std::make_shared<Sedate>(args);
    }

    double GetDeriv(double x){
        return koef * pow(x, koef - 1);
    }
};

class Exp : public TFunction{
    double base;
public:
    Exp(std::vector<double>& koefs) : base(koefs[0]){}
    double operator()(double x){return pow(base, x);}
    std::string ToString(){return std::to_string(base) + "^x";}
    TFunPtr copy(){
        std::vector<double> args = {base};
        return std::make_shared<Exp>(args);
    }
    double GetDeriv(double x){
        return log(base) * pow(base, x);
    }
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
    TFunPtr copy(){
        return std::make_shared<Polynomial>(koefs);
    }
    double GetDeriv(double x){
        double p = 0, sum = 0;
        for (auto k : koefs){
            if (p) sum += (k + p) * pow(x, p - 1);
            p++;
        }
        return sum;
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