#include "SimulatedAnnealing.hpp"

int main(int argc, char *argv[]){
    // vector<int> vec = {0, 1, 2};
    // vec.erase(vec.begin() + 2);
    // vec.insert(vec.begin() + 2, 1);
    // for (int t = 0; t < 100000; ++ t){
    //     if (rand() / ((RAND_MAX + 1u) / 10) == 10) {
    //         cout << "shit";
    //     }
    // }
    // cout <<*vec.begin() << endl;
    int shed_proc_n = atoi(argv[2]); //usually 100
    int parallel = atoi(argv[3]);
    double T0 = 10;
    int mutation_per_T = 10;
    int inner_cycle = 100;
    int outer_cycle = 10;
    int jobs_n = atoi(argv[1]);
    auto seed = time(nullptr);
    cout << "Seed " << seed<< endl;
    srand(seed);
    for (int t = 0; t < 10; t++){
        unsigned int start = clock();
        SimulatedAnnealing <CauchyLaw> simulation(shed_proc_n, 
                                                parallel, 
                                                T0,
                                                mutation_per_T, 
                                                inner_cycle, 
                                                outer_cycle);
        simulation.generate_jobs(jobs_n, "Job.xml"); 
        simulation.read_jobs("Job.xml");
        simulation.simulated_annealing();
            
        unsigned int finish = clock();
        cout << "Work time: "  << (double)(finish - start) / CLOCKS_PER_SEC << endl;
    }
    return 0;
}

