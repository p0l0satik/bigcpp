#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include <cmath>
#include <experimental/random>
#include <fstream>
#include <string>
#include <stdio.h>

using field_t = std::vector<std::vector<char> >;
using uint = unsigned int;
using namespace std::experimental;

class GameOfLife{
    field_t field;
    uint cols, rows;
public:
    void make_step();
    void print_field();
    void animate(uint steps);
    GameOfLife(uint cols, uint rows, std::vector<char>& gene);
    GameOfLife(uint cols, uint rows);
    GameOfLife(uint cols, uint rows, std::string path);

};