#include "life.hpp"
#include "genetic.hpp"
#include <ctime>
#include <iostream>

int main(int argc, char *argv[])
{
    size_t rand_seed = std::time(nullptr);
    std::cout << "Seed:" << rand_seed << std::endl;
    std::experimental::reseed(rand_seed);
    // generate_agar(184, 73); //perfect for this screen
    GameOfLife life(35, 35, "../50gen.txt");
    life.animate(5000, 80000, false);
    // life.print_field();
    // life.print_field_to_file("../50gen.txt");
    // std::cout << life.get_last_change() <<std::endl;
    // Genetic algo("../test", 4, 100, 0.8, 0.1);
    // std::cout << algo.run() << std::endl;
}