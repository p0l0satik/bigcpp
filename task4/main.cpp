#include "life.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    std::experimental::reseed(1);
    std::vector<char> gen = {1, 1, 1, 0, 0, 0, 1, 0, 1};
    GameOfLife life(10, 10, "/home/polosatik/bigcpp/task4/test_field.txt");
    life.animate(101);

}