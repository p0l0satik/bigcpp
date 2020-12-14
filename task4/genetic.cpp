#include "genetic.hpp"
PopulationLife::PopulationLife(size_t size, size_t gene_len){
    this->gene_len = gene_len;
    best_crit = 0;
    change = false;
    for (size_t i = 0; i < size; ++i){
        population.emplace_back(generate_gene(gene_len));
        assessments.emplace_back(-1);
    }
}

gene_t PopulationLife::generate_gene(size_t len){
    gene_t gene;
    for (size_t i = 0; i < len; i++){
        gene.emplace_back(randint(0, 1));
    }
    return gene;
}

void Assess::assess(std::shared_ptr<AbstPopulation> population){
    size_t field_side = sqrt(population->gene_len);
    population->weight = 0;
    population->change = false;
    for (size_t i = 0; i < population->population.size(); ++i){
        GameOfLife life(field_side, field_side, population->population[i]);
        for (int t = 0; t < 101; t++){
            life.make_step();
        }

        if (life.get_last_change() == 100) {
            population->assessments[i] = 0;
        }
        else
            population->assessments[i] = life.assess();
        
        if (life.assess() > population->best_crit){
            population->best_crit = life.assess();
            life.print_field_to_file(path + "after_100");
            life = GameOfLife(field_side, field_side, population->population[i]);
            life.print_field_to_file(path);
            population->change = true;
        }

        population->weight += population->assessments[i];

    }
}

Selection::Selection(){
    gen = std::mt19937(rd());
    gen.seed(1);
}

void Selection::select(std::shared_ptr<AbstPopulation> population){
    std::uniform_real_distribution<> dis(0, population->weight);
    std::set<int> were_selected;
    for (size_t i = 0; i < population->population.size(); ++i) {
        double p = dis(gen);
        int sum = 0;
        size_t j = 0;
        while(sum <= p){
            sum += population->assessments[j++];
        }
        j--;
        were_selected.insert(j);
    }
    population->selected.clear();
    for(auto sel :  were_selected){
        population->selected.push_back(sel);
    }
}
Crossbreeding::Crossbreeding(double p){
    gen = std::mt19937(rd());
    gen.seed(1);
    breed_p = p;
}

void Crossbreeding::crossbreed(std::shared_ptr<AbstPopulation> population){
    size_t pop_size = 0;
    std::uniform_real_distribution<> dis(0, 1);
    int selected_size = population->selected.size();
        // std::cout <<"SEL"  << selected_size << std::endl;
    while (pop_size < population->population.size()){
        size_t i = randint(0, selected_size - 1), j = randint(0, selected_size - 1);
            // std::cout << i << " " << j << std::endl;

        if (breed_p > dis(gen)){
            int bp1 = randint(0, (int)population->gene_len - 2);
            int bp2 = randint(bp1 + 1, (int)population->gene_len - 1);
            // std::cout << bp1 << " " << bp2 << std::endl;
            for (int k = bp1; k <= bp2; k++){
        // std::cout << "GL "<< k<<std::endl;
        // std::cout << population->population[i][k]<<std::endl;
        // std::cout << population->population[j][k]<<std::endl;

                std::swap(population->population[i][k], population->population[j][k]);
    // std::cout << "HF"<<std::endl;
            }
            pop_size += 2;
        }
    }

}

Mutation::Mutation(double p){
    gen = std::mt19937(rd());
    gen.seed(1);
    mutate_p = p;
}

void Mutation::mutate(std::shared_ptr<AbstPopulation> population){
    std::uniform_real_distribution<> dis(0, 1);
    for (size_t i = 0; i < population->population.size(); ++i) {
        for (size_t j = 0; j < population->gene_len; ++j){
            if (mutate_p > dis(gen)){
                population->population[i][j] = (population->population[i][j] + 1) % 2;
            }
        }
    }
}

Genetic::Genetic(std::string run, size_t pop_len, size_t gene_len, double p_breed, double p_mutate){
    assess = std::make_unique<Assess>(run);
    crbreeding = std::make_unique<Crossbreeding>(p_breed);
    mutation = std::make_unique<Mutation>(p_mutate);
    selection = std::make_unique<Selection>();
    pop = std::make_shared<PopulationLife>(pop_len, gene_len);
}
uint Genetic::run(){
    uint no_change = 0;
    while (no_change < 20){
        assess->assess(pop);

        selection->select(pop);

        crbreeding->crossbreed(pop);
        mutation->mutate(pop);
        if (pop->changed()) 
            no_change = 0;
        else
            no_change++;
    
    }
    return pop->get_best_krit();
}