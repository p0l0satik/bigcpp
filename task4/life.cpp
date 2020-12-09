#include "life.hpp"

void clear(){
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

GameOfLife::GameOfLife(uint cols, uint rows, std::vector<char>& gene):cols(cols), rows(rows){
    uint k = 0;
    for (int i = 0; i < rows; ++i){
        field.emplace_back(std::vector<char>());
        for (int j = 0; j < cols; ++j){
            field[i].emplace_back(gene[k++]);
        }
    }
}

GameOfLife::GameOfLife(uint cols, uint rows):cols(cols), rows(rows){
    uint k = 0;
    for (int i = 0; i < rows; ++i){
        field.emplace_back(std::vector<char>());
        for (int j = 0; j < cols; ++j){
            field[i].emplace_back(randint(0, 1));
        }
    }
}

GameOfLife::GameOfLife(uint cols, uint rows, std::string path):cols(cols), rows(rows){
    uint k = 0;
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

void GameOfLife::make_step(){
    std::vector<char*> die, born;
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
    for (auto d : die){
        *d = 0;
    }
    for (auto b : born){
        *b = 1;
    }
}

void GameOfLife::animate(uint steps){
    clear();
    print_field();
    for (int i = 0; i < steps; ++i){
        sleep(1);
        make_step();
        clear();
        print_field();
    }
}
