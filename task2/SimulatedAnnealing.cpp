#ifndef MYCLASS_FUNCTIONS
#include "SimulatedAnnealing.hpp"

#else


template<class TempLaw>
void SimulatedAnnealing<TempLaw>::generate_jobs(int N, string name){
    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("Root");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement * pElement = xmlDoc.NewElement("Jobs");
    pElement->SetAttribute("size", N);
    for (int i = 0; i < N; ++i)
    {
        XMLElement * pListElement = xmlDoc.NewElement("Task");
        pListElement->SetAttribute("n", i);
        pListElement->SetText(1 + rand()/((RAND_MAX + 1u)/30) + 1.0 / (rand()/((RAND_MAX + 1u)/70) + 1));
        pElement->InsertEndChild(pListElement);
    }
    pRoot->InsertEndChild(pElement);
    XMLError eResult = xmlDoc.SaveFile(name.c_str());
}

template<class TempLaw>
void SimulatedAnnealing<TempLaw>::read_jobs(string name){
    jobs.clear();
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(name.c_str());
    XMLNode * pRoot = xmlDoc.FirstChild();
    XMLElement * pElement = pRoot->FirstChildElement("Jobs");
    int N;
    pElement->QueryIntAttribute("size", &N);
    XMLElement * pListElement = pElement->FirstChildElement("Task");
    while(pListElement != nullptr){
        double iOutListValue;
	    eResult = pListElement->QueryDoubleText(&iOutListValue);
        jobs.push_back(iOutListValue);
        pListElement = pListElement->NextSiblingElement("Task");
    }
}

template<class TempLaw>
double SimulatedAnnealing<TempLaw>::start_main_cycle(string file, double best, string best_f){
    auto solution = make_shared<Solution>(jobs, proc_n, best, best_f);
    auto temp_law = make_unique<TempLaw>(start_T);
    auto mutation = make_unique<Mutation>(solution, inner_mute);
    int no_change = 0;
    int iter = 0;
    while (no_change < in_cycle){
        
        if (mutation->mutate(temp_law->get_temp())){
            no_change = 0;
        } else {
            no_change++;

        }
        temp_law->cool_down();
        iter++;
    }
    // cout << iter << endl;
    solution->print_best_solution(file);
    return solution->best_sol_assesm;
}

template<class TempLaw>
void SimulatedAnnealing<TempLaw>::simulated_annealing(){
    auto key =  ftok(".", 'F');
    ofstream cl ("best_timetable.txt");
    cl.close();
    fs::path p = fs::current_path();
    int no_change = 0;
    double min_time = -1;
    int id = semget(key, 1, 0666 | IPC_CREAT);
    if (id < 0) {
        perror("semget");
        exit(11);
    }
    union semun u;
    u.val = 1;
    if (semctl(id, 0, SETVAL, u) < 0) {
        perror("semctl");
        exit(12);
    }

    vector<int> pids;
    int k = 0;
    ofstream res ("res.txt");
        cl << "-1";
    res.close();
    while (no_change < out_cycle) {
        for (int i = 0; i < parallel_n - 1; ++i){
            int pid = fork();
            if (!pid){
                pids.clear();
                break;
            }
            k++;
            pids.push_back(pid);
        }
        srand(time(nullptr) + k);
        auto res = start_main_cycle("timetable" + to_string(k) + ".txt", min_time, "best_timetable.txt");
        
        if (semop(id, &p0, 1) < 0){
            perror("semop p");
            exit(13);
        }
        ofstream fout ("res.txt", ios_base::app);
        fout << k << " " << res << endl;
        fout.close();
        
        
        if (semop(id, &v0, 1) < 0){
            perror("semop v");
            exit(14);
        }

        if (parallel_n == 1 || pids.size()){
            int s;
            for(int t = 0; t < pids.size(); ++t) wait(&s);
            ifstream fin ("res.txt");
            int min_n = -1;
            for (int i = 0; i < parallel_n; ++i){
                double time, k;
                fin >> k >> time;
                if (min_time < 0 || time < min_time) {
                    min_time = time;
                    no_change = 0;
                    min_n = k;
                }
            }
            fin.close();

            if (min_n >= 0)
               fs::rename(p / ("timetable" + to_string(min_n) + ".txt").c_str(), p / "best_timetable.txt");
            else
                no_change++;

            for (int i = 0; i < parallel_n; ++i){
                if (i == min_n) {
                    continue;
                }
                fs::remove(p / ("timetable" + to_string(i) + ".txt").c_str());
            }

        } else {
            if (parallel_n > 1){
                exit(0);

            }

        }
        pids.clear();
        k = 0;
    }
    cout << "MIN " <<min_time <<  endl;
    fs::remove("res.txt");
}

#endif