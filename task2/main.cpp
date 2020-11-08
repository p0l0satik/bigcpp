#include "SimulatedAnnealing.hpp"

int main(void){
    SimulatedAnnealing <CauchyLaw> simulation(5);
    simulation.generate_jobs(10); 
    simulation.read_jobs();
    simulation.start_main_cycle(10, 5);
    return 0;
}

