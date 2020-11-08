#include "solution.hpp"

int gen_int(int to, int from = 0){
    return from + std::rand() / ((RAND_MAX + 1u) / to);
}

Solution::Solution(vector<double>& jobs, int M){
    best_sol_assesm = -1.0;
    int curr_proc = 0;
    for (auto job : jobs){
        if (timetable.count(curr_proc) == 0){
            timetable[curr_proc] = std::vector<double>();
        }
        timetable[curr_proc].push_back(job);
        curr_proc = (curr_proc + 1) % M;
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
void Solution::print_best_solution(){
    for (auto proc : best_solution){
        for (auto job : proc.second){
            cout << setw(6) << fixed << setprecision(2) << job << " ";
        }
        cout << endl;
    }
}

Mutation::Mutation(shared_ptr<Solution> &sol, int max_mute):solution(sol), mute(max_mute){
    gen = mt19937(rd());
    uni_dis = uniform_real_distribution<>(0, 1.0);
    srand(std::time(nullptr));
};

double Mutation::assess_solution(Timetable& timetable){
    double working_time = 0.0;
    for (auto proc : timetable){
        double proc_w_time = 0.0;
        for (auto job : proc.second){
            working_time += proc_w_time + job;
            proc_w_time += job;
        }
    }
    return working_time;
}

bool Mutation::mutate(double T){
    bool improved = false;
    for (int i = 0; i < gen_int(mute, 1); ++i){
        int proc_n = solution->timetable.size();
        double curr_assesm = assess_solution(solution->timetable);
        int proc1 = gen_int(proc_n);
        while(solution->timetable[proc1].size() == 0) {
            proc1 = gen_int(proc_n);
        }
        int proc2 = gen_int(proc_n);
        int task2 = gen_int(solution->timetable[proc2].size() + 1), task1 = gen_int(solution->timetable[proc1].size());
        solution->print_solution();
        if (solution->timetable[proc2].size() == task2){
            solution->timetable[proc2].push_back(solution->timetable[proc1][task1]);
            solution->timetable[proc1].erase(solution->timetable[proc1].begin() + task1);
        } else 
            swap(solution->timetable[proc1][task1], solution->timetable[proc2][task2]);
        cout << "after" << endl;
        solution->print_solution();
        double delta = assess_solution(solution->timetable) - curr_assesm;
        
        cout << delta << " " << curr_assesm <<  " " << exp((1) * delta / T)<< endl ;
        if (delta > 0){
            cout << endl;
            curr_assesm = assess_solution(solution->timetable);
            if (solution->best_sol_assesm < 0 || curr_assesm - solution->best_sol_assesm > 0){
                solution->best_sol_assesm = curr_assesm;
                solution->best_solution = solution->timetable;
                improved = true;
            }
        } else if (delta < 0 && uni_dis(gen) > exp((1) * delta / T)){
            std::cout << "SWAPPED BACK " << delta <<  " " << exp((1) * delta / T)<< endl;
            if (solution->timetable[proc2].size() == task2){
                solution->timetable[proc1].insert(solution->timetable[proc1].begin() + task1, solution->timetable[proc2].back());
                solution->timetable[proc2].pop_back();
            } else 
                swap(solution->timetable[proc1][task1], solution->timetable[proc2][task2]);
            cout << "after reverse" << endl;
            solution->print_solution();
        }
    }
    return improved;

}