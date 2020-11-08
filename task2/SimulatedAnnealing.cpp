#ifndef MYCLASS_FUNCTIONS
#include "SimulatedAnnealing.hpp"

#else

template<class TempLaw>
void SimulatedAnnealing<TempLaw>::generate_jobs(int N){
    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("Root");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement * pElement = xmlDoc.NewElement("Jobs");

    pElement->SetAttribute("size", N);
    for (int i = 0; i < N; ++i)
    {
        XMLElement * pListElement = xmlDoc.NewElement("Task");
        pListElement->SetAttribute("n", i);
        pListElement->SetText(1 + std::rand()/((RAND_MAX + 1u)/30) + 1.0 / (std::rand()/((RAND_MAX + 1u)/70) + 1));

        pElement->InsertEndChild(pListElement);
    }
    pRoot->InsertEndChild(pElement);
    XMLError eResult = xmlDoc.SaveFile("Jobs.xml");
}

template<class TempLaw>
void SimulatedAnnealing<TempLaw>::read_jobs(){
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile("Jobs.xml");
    XMLNode * pRoot = xmlDoc.FirstChild();
    // if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;
    XMLElement * pElement = pRoot->FirstChildElement("Jobs");
    // if (pElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
    int N;
    pElement->QueryIntAttribute("size", &N);
    XMLElement * pListElement = pElement->FirstChildElement("Task");
    while(pListElement != nullptr){
        double iOutListValue;
	    eResult = pListElement->QueryDoubleText(&iOutListValue);
        jobs.push_back(iOutListValue);
        pListElement = pListElement->NextSiblingElement("Task");
        // cout << std::rand()/((RAND_MAX + 1u)/6)<< endl;
    }
}

template<class TempLaw>
void SimulatedAnnealing<TempLaw>::start_main_cycle(double T0, int max_mute){
    auto solution = make_shared<Solution>(jobs, proc_n);
    solution->print_solution();
    auto temp_law = make_unique<TempLaw>(T0);
    auto mutation = make_unique<Mutation>(solution, max_mute);
    int no_change = 0;
    int iter = 0;
    while (no_change < 100){
        // cout << "TEMP: " << temp_law->get_temp() << endl; 
        
        if (mutation->mutate(temp_law->get_temp())){
            no_change = 0;
        } else {
            no_change++;
            // solution->print_solution();

        }
        temp_law->cool_down();
        iter++;
    }
        cout << "Iter: " << iter++ << endl;
    cout << "Result " << endl << endl;
    solution->print_solution();
    cout << "Best " <<solution->best_sol_assesm << endl << endl;
    solution->print_best_solution();
}
#endif