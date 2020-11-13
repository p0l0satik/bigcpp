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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <filesystem>
using namespace std;
using namespace tinyxml2;
namespace fs = filesystem;

#ifndef MYCLASS
#define MYCLASS
using Timetable = map<int, vector<double>>;



template<class TempLaw>
class SimulatedAnnealing{
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    };
    struct sembuf p0 = {0, -1, SEM_UNDO};
    struct sembuf v0 = {0, +1, SEM_UNDO};
    int proc_n, parallel_n, inner_mute, out_cycle, in_cycle;
    double start_T;
    vector<double> jobs;
    unique_ptr<SolutionAbstr> solution;
    unique_ptr<TemperatureLaw> temp_law;
    unique_ptr<MutationAbstr> mutation;
public:
    void generate_jobs(int N, string name);
    void read_jobs(string name);
    SimulatedAnnealing(int M, int N, double T0, int i_mute, int i_cycle, int o_cycle):
                       proc_n(M), parallel_n(N), start_T(0), inner_mute(i_mute),
                       out_cycle(o_cycle), in_cycle(i_cycle){};
    double start_main_cycle(string file, double best, string best_f);
    void simulated_annealing();


};
#define MYCLASS_FUNCTIONS
#include "SimulatedAnnealing.cpp"
#endif