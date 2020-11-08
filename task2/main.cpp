#include "SimulatedAnnealing.hpp"

int main(void){
    SimulatedAnnealing <CauchyLaw> simulation(10);
    simulation.generate_jobs(60); 
    simulation.read_jobs();
    simulation.start_main_cycle(10000, 5);
    return 0;
}

