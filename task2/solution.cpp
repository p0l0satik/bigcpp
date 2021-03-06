#include "solution.hpp"

int gen_int(int to, int from = 0){
    if (to == from) return to;
    return from + rand() / ((RAND_MAX + 1u) / to);
}

Solution::Solution(vector<double>& jobs, int M, double best, string best_f){
    if (best == -1.0) {
        best_sol_assesm = -1.0;
        for (int t = 0; t < M; t ++){
            timetable[t] = vector<double>();
            // timetable[t].resize(jobs.size());
        }
        for (auto job : jobs)
            timetable[0].push_back(job);
    } else {
        ifstream in (best_f);
            for (int t = 0; t < M; t++){

                double job;
                in >> job;
                timetable[t] = std::vector<double>();
                // timetable[t].resize(jobs.size());
                while(job >= 0){
                    timetable[t].push_back(job);
                    in >> job;
                }
            }
        in.close();
        best_solution = timetable;
        best_sol_assesm = best;
    }

}


void Solution::print_solution(){
    for (auto proc : timetable){
        for (auto job : proc.second){
            cout << setw(6) << fixed << setprecision(2) << job << " ";
        }
        cout << endl;
    }
}
void Solution::print_best_solution(string file){
    ofstream out (file, ios_base::app);
    for (auto proc : best_solution){
        for (auto job : proc.second){
            out << setw(6) << fixed << setprecision(2) << job << " ";
        }
        out << -1 << endl;
    }
    out.close();
}

SolutionAbstr::~SolutionAbstr(){
    for (auto it : timetable){
        it.second.clear();
    }
    for (auto it : best_solution){
        it.second.clear();
    }
    timetable.clear();
    best_solution.clear();
}

Mutation::Mutation(int mute):mute(mute){
    gen = mt19937(rd());
    uni_dis = uniform_real_distribution<>(0, 1.0);
    // srand(time(nullptr));
};

double Mutation::assess_solution(Timetable& timetable){
    double working_time = 0.0;
    for (auto& proc : timetable){
        double proc_w_time = 0.0;
        for (auto& job : proc.second){
            proc_w_time += job;
            working_time += proc_w_time;
        }
    }
    return working_time;
}

bool Mutation::mutate(unique_ptr<SolutionAbstr> &solution, double T){
    bool improved = false;
    for (int i = 0; i < mute; ++i){
        int proc_n = solution->timetable.size();
        double curr_assesm = assess_solution(solution->timetable);
        int proc1 = gen_int(proc_n);
        while(solution->timetable[proc1].size() == 0) {
            proc1 = gen_int(proc_n);
        }
        int proc2 = gen_int(proc_n);
        int task2 = gen_int(solution->timetable[proc2].size()), task1 = gen_int(solution->timetable[proc1].size());
        if (proc1 != proc2) {
            solution->timetable[proc2].insert(solution->timetable[proc2].begin() + task2, solution->timetable[proc1][task1]);
            solution->timetable[proc1].erase(solution->timetable[proc1].begin() + task1);
        } else {
            swap(solution->timetable[proc1][task1], solution->timetable[proc2][task2]);
        }
        
        double delta = assess_solution(solution->timetable) - curr_assesm;
        
        if (delta <= 0){
            curr_assesm = assess_solution(solution->timetable);
            if (solution->best_sol_assesm < 0 || curr_assesm - solution->best_sol_assesm < 0){

                solution->best_sol_assesm = curr_assesm;
                solution->best_solution = solution->timetable;
                improved = true;
            }
        } else if (delta > 0 && uni_dis(gen) > exp((-1) * delta / T)){
            if (proc1 != proc2) {
                solution->timetable[proc1].insert(solution->timetable[proc1].begin() + task1, solution->timetable[proc2][task2]);
                solution->timetable[proc2].erase(solution->timetable[proc2].begin() + task2);
            } else {
                swap(solution->timetable[proc1][task1], solution->timetable[proc2][task2]);
            }
        }
    }
    return improved;

}