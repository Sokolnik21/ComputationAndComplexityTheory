#include <list>
#include <fstream>
#include <string>

#include <map>
// #include <list>
#include <tuple>
#include <set>
#include <iostream>

#define IS_DEBUG_MODE_ON 1

// DTBoard.cc
struct DTBoard {
    int rows;
    int cols;
    int optimal_steps;
    std::list<std::string> cells;

    DTBoard(std::string path) {
        std::ifstream file(path);
        if(file.is_open())
            set_attributes(file);
        file.close();
    }
private:
    void set_attributes(std::ifstream &file) {
        set_board_parameters(file);
        set_board_cells(file);
    }
    void set_board_parameters(std::ifstream &file) {
        std::string line;
        getline(file, line, ' ');
        rows = std::stoi(line);
        getline(file, line, '\n');
        cols = std::stoi(line);
        getline(file, line, '\n');
        optimal_steps = std::stoi(line);
    }
    void set_board_cells(std::ifstream &file) {
        std::string line;
        while (getline(file, line)) {
            cells.push_back(line);
        }
    }
};

// Point.cc
class Point {
protected:
    std::tuple<int, int> position;
public:
    Point(int row, int col) {
        position = std::make_tuple(row, col);
    }
    std::tuple<int, int> get_position() {
        return position;
    }
};

class StartingPoint : public Point {
public:
    StartingPoint(): Point(-1, -1) { }
    void set_position(Point new_position){
        position = new_position.get_position();
    }
    bool is_starting_point_set() {
        return (std::get<0>(position) == -1) ? false : true;
    }
};

// ItemExtractor.cc
class ItemExtractor {
    const DTBoard dtb;
    StartingPoint starting_point = StartingPoint();
    std::list<Point> accessible_cells;
    std::list<Point> diamond_cells;
    std::list<Point> pit_cells;
    std::list<Point> mine_cells;

public:
    ItemExtractor(DTBoard dtb): dtb(dtb){
        set_path_finder_attributes();
    }

    int get_board_rows() {
        return dtb.rows;
    }
    int get_board_cols() {
        return dtb.cols;
    }
    int get_optimal_steps() {
        return dtb.optimal_steps;
    }
    StartingPoint get_starting_point() {
        return starting_point;
    }
    std::list<Point> get_accessible_cells() {
        return accessible_cells;
    }
    std::list<Point> get_diamond_cells() {
        return diamond_cells;
    }
    std::list<Point> get_pit_cells() {
        return pit_cells;
    }
    std::list<Point> get_mine_cells() {
        return mine_cells;
    }
    #if IS_DEBUG_MODE_ON
    void test_starting_point_parameter() {
        std::cout << "start_point_set: " << starting_point.is_starting_point_set() << '\n';
        if(starting_point.is_starting_point_set())
            std::cout << "start_point: " << "[" <<
                std::get<0>(starting_point.get_position()) << "," <<
                std::get<1>(starting_point.get_position()) << "]\n";
        // set position
        starting_point.set_position(Point(1, 2));
        std::cout << "start_point_set: " << starting_point.is_starting_point_set() << '\n';
        if(starting_point.is_starting_point_set())
            std::cout << "start_point: " << "[" <<
                std::get<0>(starting_point.get_position()) << "," <<
                std::get<1>(starting_point.get_position()) << "]\n";
    }
    void print_board_parameters() {
        std::cout << "rows: " << dtb.rows << '\n';
        std::cout << "cols: " << dtb.cols << '\n';
        std::cout << "opt_steps: " << dtb.optimal_steps << '\n';
    }
    void print_board_string_precise() {
        for(std::list<std::string>::const_iterator it_row = dtb.cells.begin(); it_row != dtb.cells.end(); ++it_row) {
                std::cout << *it_row << '\n';
        }
    }
    void print_board_char_precise() {
        for(std::list<std::string>::const_iterator it_row = dtb.cells.begin(); it_row != dtb.cells.end(); ++it_row) {
            std::string current_row(*it_row);
            for(std::string::const_iterator it_cell = current_row.begin(); it_cell != current_row.end(); ++it_cell) {
                std::cout << (char)*it_cell << ' ';
            }
            std::cout << '\n';
        }
    }
    #endif
private:
    void set_path_finder_attributes() {
        int curr_row = 0;
        int curr_col;
        for(std::list<std::string>::const_iterator it_row = dtb.cells.begin(); it_row != dtb.cells.end(); ++it_row) {
            curr_col = 0;
            std::string current_row(*it_row);
            for(std::string::const_iterator it_cell = current_row.begin(); it_cell != current_row.end(); ++it_cell) {
                Point point(curr_row, curr_col);
                if(is_cell_in_pattern(*it_cell, {' ', '+', 'O', '.'})) {
                    accessible_cells.push_back(point);
                    if(is_cell_in_pattern(*it_cell, {'+'})) {
                        diamond_cells.push_back(point);
                    }
                    if(is_cell_in_pattern(*it_cell, {'0'})) {
                        pit_cells.push_back(point);
                    }
                    if(is_cell_in_pattern(*it_cell, {'.'})) {
                        starting_point.set_position(point);
                    }
                }
                else {
                    if(is_cell_in_pattern(*it_cell, {'*'})) {
                        mine_cells.push_back(point);
                    }
                }
                ++curr_col;
            }
            ++curr_row;
        }
    }
    bool is_cell_in_pattern(char cell, std::set<char> pattern) {
        std::set<char>::iterator found_char = pattern.find(cell);
        return (found_char != pattern.end()) ? true : false;
    }
};

int main(int argc, char * argv[]) {
    std::string file_path(argv[1]);
    const DTBoard dtb = DTBoard(file_path);
    ItemExtractor pf(dtb);

    #if IS_DEBUG_MODE_ON
    pf.print_board_parameters();
    pf.print_board_string_precise();
    pf.print_board_char_precise();
    pf.test_starting_point_parameter();
    #endif

    return 0;
}
