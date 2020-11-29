#include "SimulatedAnnealing.hpp"

int main(int argc, char *argv[]){
    int shed_proc_n = atoi(argv[2]); //usually 100cd big    
    int parallel = atoi(argv[3]);
    double T0 = atoi(argv[4]);
    int mutation_per_T = 10;
    int inner_cycle = 100;
    int outer_cycle = 10;
    int jobs_n = atoi(argv[1]);
    auto seed = time(nullptr);
    // auto seed = 1605268579;
    cout << "Seed " << seed<< endl;
    srand(seed);
    for (int t = 0; t < 1; t++){
        unsigned int start = clock();
        SimulatedAnnealing <MixedLaw> simulation(shed_proc_n, 
                                                parallel, 
                                                T0,
                                                mutation_per_T, 
                                                inner_cycle, 
                                                outer_cycle);
        simulation.generate_jobs(jobs_n, "Job.xml"); 
        simulation.read_jobs("Job.xml");
        simulation.simulated_annealing();
            
        unsigned int finish = clock();
        cout << ""  << (double)(finish - start) / CLOCKS_PER_SEC << endl;
    }
    return 0;
}

