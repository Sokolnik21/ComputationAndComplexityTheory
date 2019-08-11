#include <fstream>
#include <iostream>

#include <set>
#include <list>
#include <vector>
#include <map>
#include <tuple>
#include <string>

#include <algorithm>

#define NOT_SOLVABLE "BRAK"
#define IS_TIME_EXECUTION_COUNTER_ON 0


/**
 * Module name: Data Transfer Board
 * Responsibility: Read an input file and split data into categories
 */
struct DTBoard {
    int rows;
    int cols;
    int optimal_steps;
    std::vector<std::string> cells;

    DTBoard(std::string path) {
        std::ifstream file(path);
        if(file.is_open())
            set_attributes(file);
        file.close();
    }
    static bool is_cell_in_pattern(char cell, std::set<char> pattern) {
        std::set<char>::iterator found_char = pattern.find(cell);
        return (found_char != pattern.end()) ? true : false;
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


/**
 * Module name: Point
 * Responsibility: Convert location of signs from vectors of strings into more
 * self-explanatory structure
 */
class Point {
protected:
    std::tuple<int, int> position;
public:
    Point() {};
    Point(int row, int col) {
        position = std::make_tuple(row, col);
    }
    std::tuple<int, int> get_position() {
        return position;
    }
    void set_position(Point new_position){
        position = new_position.get_position();
    }
    int get_row() {
        return std::get<0>(position);
    }
    int get_col() {
        return std::get<1>(position);
    }
    bool is_point_valid() {
        return (std::get<0>(position) < 0) ? false : true;
    }
    friend bool operator<(const Point& l, const Point& r);
    friend bool operator!=(const Point& l, const Point& r);
};

bool operator<(const Point& l, const Point& r) {
    if(std::get<0>(l.position) < std::get<0>(r.position))
        return true;
    else if(std::get<0>(l.position) == std::get<0>(r.position))
        if(std::get<1>(l.position) < std::get<1>(r.position))
            return true;
    return false;
}

bool operator!=(const Point& l, const Point& r) {
    if(std::get<0>(l.position) != std::get<0>(r.position))
        return true;
    if(std::get<1>(l.position) != std::get<1>(r.position))
        return true;
    return false;
}


/**
 * Module name: Item Extractor
 * Responsibility: Extract the location of different signs from the board and
 * exhibit lists of them for higher layer
 */
class ItemExtractor {
    const DTBoard dtb;
    // set starting point to invalid value
    Point starting_point = Point(-1, -1);
    std::list<Point> accessible_cells;
    std::list<Point> diamond_cells;
    std::list<Point> pit_cells;
    std::list<Point> mine_cells;

public:
    ItemExtractor(DTBoard dtb): dtb(dtb){
        set_item_extractor_attributes();
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
    Point get_starting_point() {
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
private:
    void set_item_extractor_attributes() {
        int row = 0;
        int col;
        std::vector<std::string>::const_iterator it_row;
        for(it_row = dtb.cells.begin(); it_row != dtb.cells.end(); ++it_row) {
            col = 0;
            std::string::const_iterator it_cell;
            for(it_cell = (*it_row).begin(); it_cell != (*it_row).end(); ++it_cell) {
                Point point(row, col);
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
                ++col;
            }
            ++row;
        }
    }
};


/**
 * Module name: Ball Pusher
 * Responsibility: Provide movement and behavior of the ball for higher layer
 */
enum Direction {
    UP = 0,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT,
    NUMBER_OF_DIRECTIONS
};

class BallPusher {
    const DTBoard dtb;
public:
    BallPusher(DTBoard dtb): dtb(dtb) { }
    std::tuple<Point, std::set<Point>> push_ball(Point ball, Direction direction) {
        std::set<Point> diamonds;
        Point curr_position = ball;
        if(is_ball_on_diamond(curr_position)) {
            diamonds.insert(curr_position);
        }
        while(is_able_to_push(curr_position, direction)) {
            curr_position = get_next_point_in_line(curr_position, direction);
            if(is_ball_on_diamond(curr_position)) {
                diamonds.insert(curr_position);
            }
            if(is_ball_in_pit(curr_position)) {
                break;
            }
        }
        if(is_able_to_stop(curr_position, direction)) {
            return std::make_tuple(curr_position, diamonds);
        }
        else {
            return std::make_tuple(ball, std::set<Point>());
        }
    }
private:
    bool is_able_to_push(Point ball, Direction direction) {
        Point destination = get_next_point_in_line(ball, direction);
        char destination_cell = dtb.cells.at(destination.get_row())[destination.get_col()];
        return DTBoard::is_cell_in_pattern(destination_cell, {' ', '+', 'O', '.'});
    }
    bool is_able_to_stop(Point ball, Direction direction) {
        char base_cell = dtb.cells.at(ball.get_row())[ball.get_col()];
        if(DTBoard::is_cell_in_pattern(base_cell, {'O'})) {
            return true;
        }
        else {
            Point destination = get_next_point_in_line(ball, direction);
            char destination_cell = dtb.cells.at(destination.get_row())[destination.get_col()];
            return DTBoard::is_cell_in_pattern(destination_cell, {'#'});
        }
    }
    Point get_next_point_in_line(Point ball, Direction direction) {
        int row = ball.get_row();
        int col = ball.get_col();
        Point destination = Point();
        switch(direction) {
            case UP :
                destination.set_position(Point(row - 1, col));
                break;
            case UP_RIGHT :
                destination.set_position(Point(row - 1, col + 1));
                break;
            case RIGHT :
                destination.set_position(Point(row, col + 1));
                break;
            case DOWN_RIGHT :
                destination.set_position(Point(row + 1, col + 1));
                break;
            case DOWN :
                destination.set_position(Point(row + 1, col));
                break;
            case DOWN_LEFT :
                destination.set_position(Point(row + 1, col - 1));
                break;
            case LEFT :
                destination.set_position(Point(row, col - 1));
                break;
            case UP_LEFT :
                destination.set_position(Point(row - 1, col - 1));
                break;
        }
        return destination;
    }
    bool is_ball_on_diamond(Point ball) {
        char cell = dtb.cells.at(ball.get_row())[ball.get_col()];
        if(DTBoard::is_cell_in_pattern(cell, {'+'}))
            return true;
        else
            return false;
    }
    bool is_ball_in_pit(Point ball) {
        char cell = dtb.cells.at(ball.get_row())[ball.get_col()];
        if(DTBoard::is_cell_in_pattern(cell, {'O'}))
            return true;
        else
            return false;
    }
};


/**
 * Module name: Graph
 * Responsibility: Transition from primitive data to graph representation
 */
class Node {
    unsigned int index;
    Direction direction;
    std::list<unsigned int> diamonds;

public:
    Node(unsigned int index, std::list<unsigned int> diamonds, Direction direction):
        index(index), diamonds(diamonds), direction(direction) {
    }
    unsigned int get_index() {
        return index;
    }
    Direction get_direction() {
        return direction;
    }
    std::list<unsigned int> get_diamonds() {
        return diamonds;
    }
};

class Graph {
    std::map<Point, unsigned int> points_to_indexes;
    std::map<unsigned int, std::vector<Node>> graph;
    int optimal_steps;
    unsigned int start_vertex;
    std::list<unsigned int> diamonds;

public:
    Graph(DTBoard dtb) {
        BallPusher bp(dtb);
        ItemExtractor ie(dtb);

        std::list<Point> listof_accessible_cells = ie.get_accessible_cells();
        points_to_indexes = get_points_mapping(listof_accessible_cells);

        std::vector<Node> neighbours;
        std::list<Point>::iterator it;
        for (it = listof_accessible_cells.begin(); it != listof_accessible_cells.end(); ++it) {
            neighbours.clear();
            for(int i = 0; i < Direction::NUMBER_OF_DIRECTIONS; i++) {
                std::tuple<Point, std::set<Point>> connected_vertex = bp.push_ball(*it, static_cast<Direction>(i));
                if(*it != std::get<0>(connected_vertex)) {
                    unsigned int next_vertex = get_index_from_point(std::get<0>(connected_vertex));
                    std::list<unsigned int> diamonds_on_edge;
                    std::list<Point> diamonds_on_edge_list(std::get<1>(connected_vertex).begin(), std::get<1>(connected_vertex).end());
                    std::list<Point>::iterator it;
                    for (it = diamonds_on_edge_list.begin(); it != diamonds_on_edge_list.end(); ++it) {
                        diamonds_on_edge.push_back(get_index_from_point(*it));
                    }
                    Node new_node(next_vertex, diamonds_on_edge, static_cast<Direction>(i));
                    neighbours.push_back(new_node);
                }
            }
            graph[get_index_from_point(*it)] = neighbours;
        }
        optimal_steps = ie.get_optimal_steps();
        start_vertex = get_index_from_point(ie.get_starting_point());

        std::list<Point> diamond_points = ie.get_diamond_cells();
        for (it = diamond_points.begin(); it != diamond_points.end(); ++it) {
            diamonds.push_back(get_index_from_point(*it));
        }
    }
    std::map<unsigned int, std::vector<Node>> get_graph() {
        return graph;
    }
    std::list<unsigned int> get_diamonds() {
        return diamonds;
    }
    unsigned int get_optimal_steps() {
        return optimal_steps;
    }
    unsigned int get_starting_vertex() {
        return start_vertex;
    }

private:
    std::map<Point, unsigned int> get_points_mapping(std::list<Point> points_list) {
        std::map<Point, unsigned int> result;
        // start from 1 not from 0; result 0 from map represents error
        unsigned int index = 1;
        std::list<Point>::iterator it;
        for (it = points_list.begin(); it != points_list.end(); ++it) {
            result[*it] = index++;
        }
        return result;
    }
    unsigned int get_index_from_point(Point point) {
        return points_to_indexes[point];
    }
};


/**
 * Module name: Solver
 * Responsibility: Main functionality. Solver
 */
class Solver {
    Graph graph;
    std::map<unsigned int, std::vector<Node>> graph_ref;
    std::list<unsigned int> diamonds;
public:
    Solver(Graph graph): graph(graph) {
        graph_ref = graph.get_graph();
        diamonds = graph.get_diamonds();
    }
    std::string find_best_path() {
        std::string result = "";

        unsigned int max_iteration = graph.get_optimal_steps();
        unsigned int start_vertex = graph.get_starting_vertex();

        result = naive_solver(result, diamonds, start_vertex, max_iteration);
        return result;
    }
private:
    std::string naive_solver(std::string result, std::list<unsigned int> diamonds, unsigned int start_vertex, unsigned int max_iteration) {
        return go_to_next_vertex(result, diamonds, start_vertex, max_iteration);
    }
    std::string go_to_next_vertex(std::string result, std::list<unsigned int> curr_diamonds, unsigned int curr_vertex, unsigned int curr_iteration) {
        // dbg_list_diamonds(curr_diamonds);
        if(curr_diamonds.empty()) {
            return result;
        }
        if(curr_iteration == 0) {
            return NOT_SOLVABLE;
        }

        std::string tmp;

        std::list<unsigned int> new_diamonds;
        std::vector<Node> curr_node = graph_ref[curr_vertex];
        std::vector<Node>::iterator it;

        for(it = curr_node.begin(); it != curr_node.end(); ++it) {
            new_diamonds = remove_on_path(curr_diamonds, *it);
            tmp = go_to_next_vertex(result + std::to_string((*it).get_direction()), new_diamonds, (*it).get_index(), curr_iteration - 1);

            if(tmp == NOT_SOLVABLE) {
                continue;
            } else {
                return tmp;
            }
        }
        return NOT_SOLVABLE;
    }
    std::list<unsigned int> remove_on_path(std::list<unsigned int> curr_diamonds_base, Node node) {
        std::list<unsigned int> diam_on_path = node.get_diamonds();
        std::list<unsigned int> curr_diamonds = curr_diamonds_base;
        std::list<unsigned int>::iterator diam_iter;

        for(diam_iter = diam_on_path.begin(); diam_iter != diam_on_path.end(); ++diam_iter) {
            if(!diam_on_path.empty()) {
                if(std::find(diam_on_path.begin(), diam_on_path.end(), (*diam_iter)) != diam_on_path.end()) {
                    curr_diamonds.remove(*diam_iter);
                }
            }
        }

        return curr_diamonds;
    }
    void dbg_list_diamonds(std::list<unsigned int> diamonds) {
        std::cout << "diam_list: ";
        std::list<unsigned int>::iterator it;
        for(it = diamonds.begin(); it != diamonds.end(); ++it) {
            std::cout << (*it) << ',';
        }
        std::cout << '\n';
    }
};

#if IS_TIME_EXECUTION_COUNTER_ON
    #include <chrono>
#endif
int main(int argc, char * argv[]) {
    std::string file_path(argv[1]);
    const DTBoard dtb = DTBoard(file_path);
    Graph g(dtb);
    Solver s(g);

    #if IS_TIME_EXECUTION_COUNTER_ON
        auto start = std::chrono::high_resolution_clock::now();
    #endif
    std::cout << s.find_best_path() << '\n';
    #if IS_TIME_EXECUTION_COUNTER_ON
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        std::cout << "Execution time (in seconds): " << duration.count() << std::endl;
    #endif

    return 0;
}
