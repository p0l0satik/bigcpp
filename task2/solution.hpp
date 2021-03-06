#pragma once
#include <map>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <random>

#include <fstream>
using namespace std;
using Timetable = map<int, vector<double>>;



class SolutionAbstr;

class MutationAbstr{
protected:
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> uni_dis;
public:
    virtual bool mutate(unique_ptr<SolutionAbstr> &solution, double T) = 0;
    virtual double assess_solution(Timetable& timetable) = 0;
    virtual ~MutationAbstr() = default;
};

class Mutation: public MutationAbstr{
    int mute;
public:
    bool mutate(unique_ptr<SolutionAbstr> &solution, double T);
    double assess_solution(Timetable& timetable);
    Mutation(int mute);
    ~Mutation() = default;
};

class SolutionAbstr{
protected:
    Timetable timetable, best_solution;
public:
    double best_sol_assesm;
    virtual void print_solution() = 0;
    virtual void print_best_solution(string file) = 0;
    // virtual void read_best_solution(string file) = 0;
    friend bool Mutation::mutate(unique_ptr<SolutionAbstr> &solution, double T);
    virtual ~SolutionAbstr();
};

class Solution: public SolutionAbstr{
public:
    void print_solution();
    void print_best_solution(string file);
    // void read_best_solution(string file);
    Solution(vector<double>& jobs, int M, double best, string best_f);
    ~Solution() = default;
};
