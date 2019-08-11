#include "ItemExtractor.h"

#include "Utilities.h"

ItemExtractor::ItemExtractor(DTBoard dtb): dtb(dtb){
    set_item_extractor_attributes();
}

int ItemExtractor::get_board_rows() {
    return dtb.rows;
}
int ItemExtractor::get_board_cols() {
    return dtb.cols;
}
int ItemExtractor::get_optimal_steps() {
    return dtb.optimal_steps;
}
Point ItemExtractor::get_starting_point() {
    return starting_point;
}
std::list<Point> ItemExtractor::get_accessible_cells() {
    return accessible_cells;
}
std::list<Point> ItemExtractor::get_diamond_cells() {
    return diamond_cells;
}
std::list<Point> ItemExtractor::get_pit_cells() {
    return pit_cells;
}
std::list<Point> ItemExtractor::get_mine_cells() {
    return mine_cells;
}
#if IS_DEBUG_ITEM_EXTRACTOR_ON
void ItemExtractor::test_starting_point_parameter() {
    std::cout << "start_point_set: " << starting_point.is_point_valid() << '\n';
    if(starting_point.is_point_valid())
        std::cout << "start_point: " << "[" <<
            std::get<0>(starting_point.get_position()) << "," <<
            std::get<1>(starting_point.get_position()) << "]\n";
    // set position
    starting_point.set_position(Point(1, 2));
    std::cout << "start_point_set: " << starting_point.is_point_valid() << '\n';
    if(starting_point.is_point_valid())
        std::cout << "start_point: " << "[" <<
            std::get<0>(starting_point.get_position()) << "," <<
            std::get<1>(starting_point.get_position()) << "]\n";
}
void ItemExtractor::print_board_parameters() {
    std::cout << "rows: " << dtb.rows << '\n';
    std::cout << "cols: " << dtb.cols << '\n';
    std::cout << "opt_steps: " << dtb.optimal_steps << '\n';
}
void ItemExtractor::print_board_string_precise() {
    for(std::vector<std::string>::const_iterator it_row = dtb.cells.begin(); it_row != dtb.cells.end(); ++it_row) {
            std::cout << *it_row << '\n';
    }
}
void ItemExtractor::print_board_char_precise() {
    for(std::vector<std::string>::const_iterator it_row = dtb.cells.begin(); it_row != dtb.cells.end(); ++it_row) {
        std::string current_row(*it_row);
        for(std::string::const_iterator it_cell = current_row.begin(); it_cell != current_row.end(); ++it_cell) {
            std::cout << (char)*it_cell << ' ';
        }
        std::cout << '\n';
    }
}
#endif

void ItemExtractor::set_item_extractor_attributes() {
    int curr_row = 0;
    int curr_col;
    for(std::vector<std::string>::const_iterator it_row = dtb.cells.begin(); it_row != dtb.cells.end(); ++it_row) {
        curr_col = 0;
        std::string current_row(*it_row);
        for(std::string::const_iterator it_cell = current_row.begin(); it_cell != current_row.end(); ++it_cell) {
            Point point(curr_row, curr_col);
            if(DTBoard::is_cell_in_pattern(*it_cell, {' ', '+', 'O', '.'})) {
                accessible_cells.push_back(point);
                if(DTBoard::is_cell_in_pattern(*it_cell, {'+'})) {
                    diamond_cells.push_back(point);
                }
                if(DTBoard::is_cell_in_pattern(*it_cell, {'0'})) {
                    pit_cells.push_back(point);
                }
                if(DTBoard::is_cell_in_pattern(*it_cell, {'.'})) {
                    starting_point.set_position(point);
                }
            }
            else {
                if(DTBoard::is_cell_in_pattern(*it_cell, {'*'})) {
                    mine_cells.push_back(point);
                }
            }
            ++curr_col;
        }
        ++curr_row;
    }
}

#if IS_DEBUG_ITEM_EXTRACTOR_ON
int main(int argc, char * argv[]) {
    std::string file_path(argv[1]);
    const DTBoard dtb = DTBoard(file_path);
    ItemExtractor pf(dtb);

    pf.print_board_parameters();
    pf.print_board_string_precise();
    pf.print_board_char_precise();
    pf.test_starting_point_parameter();

    return 0;
}
#endif
