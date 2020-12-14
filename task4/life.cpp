#include "life.hpp"

void clear(){
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

void generate_agar(uint width, uint height){
    std::ofstream agar("../agar.txt");
    for (int t = 0; t < height / 3; t++){
        for (int i = 0; i < width; i++){
            agar << '.';
        }
        agar << std::endl;
        for (int i = 0; i < width; i++){
            if (i % 3 == 0) 
                agar << '.';
            else 
                agar << '#';
        }
        agar << std::endl;
        for (int i = 0; i < width; i++){
            if (i % 3 == 0) 
                agar << '.';
            else 
                agar << '#';
        }
        agar << std::endl;
    }
    for (int t = 0; t < height % 3; t++){
        for (int i = 0; i < width; i++){
            agar << '.';
        }
        agar << std::endl;
    }
}

GameOfLife::GameOfLife(uint cols, uint rows, std::vector<char>& gene):cols(cols), rows(rows){
    uint k = 0;
    iter = 0;
    last_change = 0;
    for (int i = 0; i < rows; ++i){
        field.emplace_back(std::vector<char>());
        for (int j = 0; j < cols; ++j){
            field[i].emplace_back(gene[k++]);
        }
    }
}

GameOfLife::GameOfLife(uint cols, uint rows):cols(cols), rows(rows){
    uint k = 0;
    iter = 0;
    last_change = 0;
    for (int i = 0; i < rows; ++i){
        field.emplace_back(std::vector<char>());
        for (int j = 0; j < cols; ++j){
            field[i].emplace_back(randint(0, 1));
        }
    }
}

GameOfLife::GameOfLife(uint cols, uint rows, std::string path):cols(cols), rows(rows){
    uint k = 0;
    iter = 0;
    last_change = 0;
    std::ifstream f (path);
    for (int i = 0; i < rows; ++i){
        field.emplace_back(std::vector<char>());
        for (int j = 0; j < cols; ++j){
            char c;
            f >> c;
            field[i].emplace_back(c == '#' ? 1 : 0);
        }
    }
    f.close();
}

void GameOfLife::print_field(){
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            if (field[i][j] == 1) std::cout << '#'; else std::cout << '.';
            if (j < cols - 1) std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void GameOfLife::print_field_to_file(std::string filename){
    std::ofstream f (filename);
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            if (field[i][j] == 1) f << '#'; else f << '.';
            // if (j < cols - 1) f << " ";
        }
        f << std::endl;
    }
}

void GameOfLife::make_step(){
    std::vector<char*> die, born;
    iter++;
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            uint sum = 0;
            if (i) sum += field[i-1][j];
            if (i && j) sum += field[i-1][j-1];
            if (i && j < cols - 1) sum += field[i-1][j+1];
            if (j) sum += field[i][j-1];
            if (j < cols - 1) sum += field[i][j+1];
            if (i < rows - 1) sum += field[i+1][j];
            if (i < rows - 1 && j) sum += field[i+1][j-1];
            if (i < rows - 1 && j < cols - 1) sum += field[i+1][j+1];
            
            if (field[i][j] && (sum < 2 || sum > 3)) die.push_back(&field[i][j]);
            if (field[i][j] == 0 && sum == 3) born.push_back(&field[i][j]);
            
        }
    }
    if (die.size() || born.size()) last_change = iter;
    for (auto d : die){
        *d = 0;
    }
    for (auto b : born){
        *b = 1;
    }
}

void GameOfLife::animate(uint steps, int sleep_time, bool by_step){
    clear();
    print_field();
    std::cout << "Iteration: " << iter << std::endl;
    if (by_step){
            char c;
            std::cout << "Press any simbol + enter to continue" << std::endl;
            std::cin >> c;
        }
    for (int i = 0; i < steps; ++i){
        if (not by_step) usleep(sleep_time);
        make_step();
        clear();
        print_field();
        std::cout << "Iteration: " << iter << std::endl;
        if (by_step){
            char c;
            std::cout << "Press any simbol + enter to continue" << std::endl;
            std::cin >> c;
        }
    }
}

int GameOfLife::assess(){
    uint sum = 0;
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            sum += field[i][j];
        }
    }
    return sum;
}