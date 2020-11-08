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
using namespace std;
using Timetable = map<int, vector<double>>;



class Solution;

class MutationAbstr{
protected:
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> uni_dis;
public:
    virtual bool mutate(double T) = 0;
    virtual double assess_solution(Timetable& timetable) = 0;
};

class Mutation: public MutationAbstr{
    shared_ptr<Solution> solution;
    int mute;
public:
    bool mutate(double T);
    double assess_solution(Timetable& timetable);
    Mutation(shared_ptr<Solution> &sol, int max_mute);
};

class SolutionAbstr{
protected:
    Timetable timetable, best_solution;
public:
    double best_sol_assesm;
    virtual void print_solution() = 0;
    virtual void print_best_solution() = 0;
    // SolutionAbstr(map<int, double>& jobs);
    // ~SolutionAbstr();
};

class Solution: public SolutionAbstr{
public:
    void print_solution();
    void print_best_solution();
    Solution(vector<double>& jobs, int M);
    friend bool Mutation::mutate(double T);
    // ~Solution();
};