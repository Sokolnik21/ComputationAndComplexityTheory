#ifndef DT_BOARD
#define DT_BOARD

#include <vector>
#include <fstream>
#include <string>

#include "DTBoard.h"

struct DTBoard {
    int rows;
    int cols;
    int optimal_steps;
    std::vector<std::string> cells;

    DTBoard(std::string path);
    static bool is_cell_in_pattern(char cell, std::set<char> pattern);
private:
    void set_attributes(std::ifstream &file);
    void set_board_parameters(std::ifstream &file);
    void set_board_cells(std::ifstream &file);
};

#endif
