#pragma once
#include <cmath>
#include <iostream>
using namespace std;
class TemperatureLaw{
protected:
    int iter;
    double T, T0;
public:
    TemperatureLaw(double T0):T(T0), T0(T0), iter(1){};

    virtual void cool_down() = 0;
    virtual double get_temp() = 0;
};

class BoltzmannLaw: public TemperatureLaw{
public:
    BoltzmannLaw(double T0):TemperatureLaw(T0){};
    void cool_down();
    double get_temp();
};

class CauchyLaw: public TemperatureLaw{
public:
    CauchyLaw(double T0):TemperatureLaw(T0){};
    void cool_down();
    double get_temp();
};

class MixedLaw: public TemperatureLaw{
public:
    MixedLaw(double T0):TemperatureLaw(T0){};
    void cool_down();
    double get_temp();
};
