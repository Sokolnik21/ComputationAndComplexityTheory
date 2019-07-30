#include <map>
#include "BallPusher.h"
#include "ItemExtractor.h"

class Node {
#if IS_DEBUG_GRAPH_ON
public:
#endif
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
#if IS_DEBUG_GRAPH_ON
public:
#else
private:
#endif
    std::map<Point, unsigned int> points_to_indexes;
    std::map<unsigned int, std::vector<Node>> graph;
    int optimal_steps;
    unsigned int start_vertex;

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
                    // std::cout << get_index_from_point(*it) << ": " << get_index_from_point(std::get<0>(connected_vertex)) << ", " << i << '\n';
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
    }
    std::map<unsigned int, std::vector<Node>> get_graph() {
        return graph;
    };
    unsigned int get_optimal_steps() {
        return optimal_steps;
    }
    unsigned int get_starting_vertex() {
        return start_vertex;
    }

#if IS_DEBUG_GRAPH_ON
public:
#else
private:
#endif
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

#if IS_DEBUG_GRAPH_ON
int main(int argc, char * argv[]) {
    std::string file_path(argv[1]);
    const DTBoard dtb = DTBoard(file_path);
    BallPusher bp(dtb);
    ItemExtractor ie(dtb);

    Graph g(dtb);
    std::map<unsigned int, std::vector<Node>> g_ref = g.get_graph();

    for(int i = 1; i < g_ref.size() + 1; i++) {
        std::vector<Node> n = g_ref[i];
        std::cout << i << ": ";
        std::vector<Node>::iterator it2;
        for (it2 = n.begin(); it2 != n.end(); ++it2){
            std::cout << '<' << it2->index << "," << it2->direction << ",[";
            std::list<unsigned int>::iterator it3;
            for (it3 = it2->diamonds.begin(); it3 != it2->diamonds.end(); ++it3){
                std::cout << *it3 << ",";
            }
            std::cout << "]> ";
        }
        std::cout << '\n';
    }

    std::cout << "optimal: " << g.get_optimal_steps() << '\n';
    std::cout << "start: " << g.get_starting_vertex() << '\n';
    return 0;
}
#endif
