#include "Graph.cc"
#include <algorithm>

#if IS_TIME_EXECUTION_COUNTER_ON
    #include <chrono>
#endif

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

        result = go_to_next_vertex(result, diamonds, start_vertex, max_iteration);
        return result;
    }
private:
    std::string go_to_next_vertex(std::string result, std::list<unsigned int> curr_diamonds, unsigned int curr_vertex, unsigned int curr_iteration) {
        // dbg_list_diamonds(curr_diamonds);
        if(curr_diamonds.empty()) {
            return result;
        }
        if(curr_iteration == 0) {
            return "Unsolvable";
        }

        std::string tmp;

        std::list<unsigned int> new_diamonds;
        std::vector<Node> curr_node = graph_ref[curr_vertex];
        std::vector<Node>::iterator it;

        for(it = curr_node.begin(); it != curr_node.end(); ++it) {
            new_diamonds = remove_on_path(curr_diamonds, *it);
            tmp = go_to_next_vertex(result + std::to_string((*it).get_direction()), new_diamonds, (*it).get_index(), curr_iteration - 1);

            if(tmp == "Unsolvable") {
                continue;
            } else {
                return tmp;
            }
        }
        return "Unsolvable";
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
