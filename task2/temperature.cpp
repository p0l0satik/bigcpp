#include "temperature.hpp"
void BoltzmannLaw::cool_down(){
    T = T / log(1 + iter++);
}

void CauchyLaw::cool_down(){
    T = T / (1 + iter++);
}

void MixedLaw::cool_down(){
    T = T * (log(1 + iter) / (iter + 1));
    iter++;
}

double BoltzmannLaw::get_temp(){
    return T;
}

double CauchyLaw::get_temp(){
    return T;
}

double MixedLaw::get_temp(){
    return T;
}