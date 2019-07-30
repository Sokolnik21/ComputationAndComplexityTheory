#include "Graph.cc"

class Solver {
    Graph graph;
    std::map<unsigned int, std::vector<Node>> graph_ref;
public:
    Solver(Graph graph): graph(graph) {
        graph_ref = graph.get_graph();
    }
    std::string find_best_path() {
        std::string result = "";

        unsigned int max_iteration = graph.get_optimal_steps();
        unsigned int start_vertex = graph.get_starting_vertex();

        result = go_to_next_vertex(start_vertex, max_iteration);
        return result;
    }
private:
    std::string go_to_next_vertex(unsigned int curr_vertex, unsigned int current_iter) {
        return "hi";
        std::vector<Node>::iterator it;
        // for(int i = 0; i < 1000000000000; i++) {
        //     i = i * 13;
        //     i = i / 13;
        //     graph.get_graph();
        // }

        // for (it = listof_accessible_cells.begin(); it != listof_accessible_cells.end(); ++it) {
        // }

    }

};

#if IS_DEBUG_SOLVER_ON
int main(int argc, char * argv[]) {
    std::string file_path(argv[1]);
    const DTBoard dtb = DTBoard(file_path);
    Graph g(dtb);
    Solver s(g);
    // std::string result = s.find_best_path();

    // high cost operation!
    for(int i = 0; i < 1000000; i++) {
        // g.get_graph();
        g.get_optimal_steps();
    }

    std::cout << Direction::LEFT << '\n';

    std::string str = std::to_string(Direction::LEFT) + std::to_string(Direction::LEFT);
    std::cout << str << '\n';

    std::cout << s.find_best_path() << '\n';

    return 0;
}
#endif
