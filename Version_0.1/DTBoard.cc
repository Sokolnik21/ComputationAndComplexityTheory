#include <set>
#include "DTBoard.h"

DTBoard::DTBoard(std::string path) {
    std::ifstream file(path);
    if(file.is_open())
        set_attributes(file);
    file.close();
}

void DTBoard::set_attributes(std::ifstream &file) {
    set_board_parameters(file);
    set_board_cells(file);
}

void DTBoard::set_board_parameters(std::ifstream &file) {
    std::string line;
    getline(file, line, ' ');
    rows = std::stoi(line);
    getline(file, line, '\n');
    cols = std::stoi(line);
    getline(file, line, '\n');
    optimal_steps = std::stoi(line);
}

void DTBoard::set_board_cells(std::ifstream &file) {
    std::string line;
    while (getline(file, line)) {
        cells.push_back(line);
    }
}

bool DTBoard::is_cell_in_pattern(char cell, std::set<char> pattern) {
    std::set<char>::iterator found_char = pattern.find(cell);
    return (found_char != pattern.end()) ? true : false;
}

#include "Utilities.h"
#if IS_DEBUG_DTBOARD_ON
#include <iostream>
#endif

#if IS_DEBUG_DTBOARD_ON
int main(int argc, char * argv[]) {
    std::string file_path(argv[1]);
    const DTBoard dtb = DTBoard(file_path);
    std::cout << dtb.rows << '\n';
    std::cout << dtb.cols << '\n';
    std::cout << dtb.optimal_steps << '\n';

    for (std::vector<std::string>::const_iterator it = dtb.cells.begin(); it != dtb.cells.end(); ++it)
        std::cout << *it << '\n';

    return 0;
}
#endif
