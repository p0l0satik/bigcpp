#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include <cmath>
#include <experimental/random>
#include <fstream>
#include <string>
#include <stdio.h>

void generate_agar(uint width, uint height);

using field_t = std::vector<std::vector<char> >;
using uint = unsigned int;
using namespace std::experimental;

class GameOfLife{
    field_t field;
    uint cols, rows;
    uint iter, last_change;
public:
    void make_step();
    void print_field();
    void print_field_to_file(std::string filename);
    void animate(uint steps, int sleep_time, bool by_step);
    int assess();
    uint get_iter(){return iter;}
    uint get_last_change(){return last_change;}
    GameOfLife(uint cols, uint rows, std::vector<char>& gene);
    GameOfLife(uint cols, uint rows);
    GameOfLife(uint cols, uint rows, std::string path);

};