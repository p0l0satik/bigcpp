#include "life.hpp"
#include "genetic.hpp"
#include <ctime>
#include <iostream>

int main(int argc, char *argv[])
{   

    if (std::string(argv[1]) == "exp"){
        std::ofstream best_crits("../exp/best_crits.txt");
        std::ofstream worst_time("../exp/worst_time.txt");
        std::ofstream deviation("../exp/deviation.txt");
        double init_mutate = 1.0 / (50.0 * 50.0);
        for (int i = 0; i < 10; i++){
            std::string path_pref = "../exp/series_" + std::to_string(i) + "/";
            std::ofstream series_out (path_pref + "crit.txt");
            std::ofstream series_time (path_pref + "time.txt");
            double curr_mut = init_mutate * (pow(1.5, i));
            int best_cr = -1, worst_cr = -1;
            double w_time = 0; 
            for (int ex = 0; ex < 10; ex++){
                size_t rand_seed =  clock();
                std::experimental::reseed(rand_seed);
                auto start = clock();
                std::string run = path_pref + "run_" + std::to_string(ex);
                int pop_len = 100, gene_len = 250;
                double p_breed = 0.8;
                Genetic algo(run, pop_len, gene_len, rand_seed, p_breed, curr_mut);
                auto crit = algo.run();
                auto end = clock();
                series_out << crit << std::endl;
                auto time = (double)(end - start) / CLOCKS_PER_SEC;
                series_time << time << std::endl;
                if (time > w_time) w_time = time;
                if (best_cr == -1 || best_cr < crit) best_cr = crit;
                if (worst_cr == -1 || worst_cr > crit) worst_cr = crit;
            }
            best_crits << best_cr << std::endl;
            deviation << best_cr - worst_cr << std::endl;
            worst_time << w_time << std::endl;
            series_out.close();
            series_time.close();
        }
        best_crits.close();
        worst_time.close();
        deviation.close();
    } else {
        size_t rand_seed = std::time(nullptr);
        std::cout << "Seed:" << rand_seed << std::endl;
        // std::experimental::reseed(rand_seed);
        std::experimental::reseed(1608194012);

        // generate_agar(184, 73); //perfect for this screen
        // life.print_field();
        // life.print_field_to_file("../50gen.txt");
        // std::cout << life.get_last_change() <<std::endl;
        Genetic algo("../test", 6, 25, rand_seed, 0.8, 0.1);
        std::cout << algo.run() << std::endl;
        int answer = 0;
        std::cout << "Animate solution?" << std::endl << "1 - yes" << std::endl<< "0 - no"<< std::endl;
        std::cin >> answer;
        while(answer != 0 && answer != 1){
            std::cout << "Wrong input!";
            std::cin >> answer;
        } 
        if (answer){
            GameOfLife life(5, 5, "../test");
            life.animate(120, 150000, false);
        }
    }
}