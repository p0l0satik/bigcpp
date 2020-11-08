#include "temperature.hpp"
void BoltzmannLaw::cool_down(){
    T = T0 / log(++iter);
}

void CauchyLaw::cool_down(){
    T = T0 / (++iter);
}

void MixedLaw::cool_down(){
    T = T0 * (log(++iter) / (iter));
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