#include "SimulatedAnnealing.hpp"

int main(void){
    SimulatedAnnealing <CauchyLaw> simulation(3, 200);
    simulation.generate_jobs(10); 
    simulation.read_jobs();
    simulation.start_main_cycle(10000, 5);
    return 0;
}

