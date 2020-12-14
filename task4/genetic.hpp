#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <experimental/random>
#include <memory>
#include "life.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <set>

#define FIELD_SIZE 10

using namespace std::experimental;
using gene_t = std::vector<char>;
using pop_t = std::vector<gene_t>;

class AbstPopulation;

class AbstAssess{
public:
    virtual void assess(std::shared_ptr<AbstPopulation> population) = 0;
};

class Assess : public AbstAssess{
    std::string path;
public:
    Assess(std::string &path):path(path){};
    virtual void assess(std::shared_ptr<AbstPopulation> population);
};

class AbstSelection{
public:
    virtual void select(std::shared_ptr<AbstPopulation> population) = 0;
    AbstSelection() = default;
    virtual ~AbstSelection() = default;
};

class Selection : public AbstSelection{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
public:
    Selection();
    void select(std::shared_ptr<AbstPopulation> population);
    ~Selection() = default;
};

class AbstCrossbreeding{
protected:
    double breed_p;
public:
    virtual void crossbreed(std::shared_ptr<AbstPopulation> population) = 0;
};

class Crossbreeding : public AbstCrossbreeding{
    std::random_device rd;
    std::mt19937 gen;
public:
    Crossbreeding(double p);
    void crossbreed(std::shared_ptr<AbstPopulation> population);
};

class AbstMutation{
protected:
    double mutate_p;
public:
    virtual void mutate(std::shared_ptr<AbstPopulation> population) = 0;
};

class Mutation : public AbstMutation{
    std::random_device rd;
    std::mt19937 gen;
public:
    Mutation(double p);
    void mutate(std::shared_ptr<AbstPopulation> population);
};

class AbstPopulation{
protected:
    std::vector<int> assessments, selected;
    bool change;
    uint weight, best_crit;
    pop_t population;
    size_t gene_len;
    virtual gene_t generate_gene(size_t len) = 0;
public:
    friend void Assess::assess(std::shared_ptr<AbstPopulation> population);
    friend void Selection::select(std::shared_ptr<AbstPopulation> population);
    friend void Crossbreeding::crossbreed(std::shared_ptr<AbstPopulation> population);
    friend void Mutation::mutate(std::shared_ptr<AbstPopulation> population);
    virtual uint get_best_krit(){return best_crit;}
    virtual bool changed(){return change;}
};

class PopulationLife : public AbstPopulation{
    
protected:
    gene_t generate_gene(size_t len);

public:
    PopulationLife(size_t size, size_t gene_len);

};

class Genetic{
    std::unique_ptr<AbstAssess> assess;
    std::unique_ptr<AbstCrossbreeding> crbreeding;
    std::unique_ptr<AbstMutation> mutation;
    std::unique_ptr<AbstSelection> selection;
    std::shared_ptr<AbstPopulation> pop;
public:
    Genetic(std::string run, size_t pop_len, size_t gene_len, double p_breed, double p_mutate);
    uint run();
};