#ifndef ITEM_EXTRACTOR
#define ITEM_EXTRACTOR

#include <map>
#include <list>
#include <tuple>
#include <set>
#include <iostream>

#include "DTBoard.h"
#include "Point.h"

#include "Utilities.h"

class ItemExtractor {
    const DTBoard dtb;
    // set starting point to invalid value
    Point starting_point = Point(-1, -1);
    std::list<Point> accessible_cells;
    std::list<Point> diamond_cells;
    std::list<Point> pit_cells;
    std::list<Point> mine_cells;

public:
    ItemExtractor(DTBoard dtb);

    int get_board_rows();
    int get_board_cols();
    int get_optimal_steps();
    Point get_starting_point();
    std::list<Point> get_accessible_cells();
    std::list<Point> get_diamond_cells();
    std::list<Point> get_pit_cells();
    std::list<Point> get_mine_cells();
    #if IS_DEBUG_ITEM_EXTRACTOR_ON
    void test_starting_point_parameter();
    void print_board_parameters();
    void print_board_string_precise();
    void print_board_char_precise();
    #endif
private:
    void set_item_extractor_attributes();
};

#endif
