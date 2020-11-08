#pragma once
#include <map>
#include <vector>
#include "tinyxml2.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <memory>
#include "solution.hpp"
#include "temperature.hpp"
using namespace std;
using namespace tinyxml2;

#ifndef MYCLASS
#define MYCLASS
using Timetable = map<int, vector<double>>;


template<class TempLaw>
class SimulatedAnnealing{
    double T;
    int proc_n, temp_law;
    vector<double> jobs;
public:
    void generate_jobs(int N);
    void read_jobs();
    // SimulatedAnnealing(int M, double T0):proc_n(M), T(T0){ srand(std::time(nullptr)); };
    SimulatedAnnealing(int M, double T0):proc_n(M), T(T0){};
    void start_main_cycle(double T0, int max_mute);


};
#define MYCLASS_FUNCTIONS
#include "SimulatedAnnealing.cpp"
#endif