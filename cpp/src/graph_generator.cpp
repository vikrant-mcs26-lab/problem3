#include <graph.hpp>
#include <iostream>
#include <filesystem>
#include <cstdlib>

int main(int argc, char **argv){
    if (argc < 5){
        std::cout << "Usage: ./graph_generator <seed> <num_nodes> <step_size> <directory>" << std::endl;
        return 1;
    }

    std::filesystem::path directory = argv[4];
    int seed = atoi(argv[1]);
    int num_nodes = atoi(argv[2]);
    int step_size = atoi(argv[3]);

    int max_edges = num_nodes * (num_nodes - 1) / 2;
    
    if (!std::filesystem::is_directory(directory)){
        std::cout << "Directory does not exist." << std::endl;
        return 1;
    }

    directory = directory / std::filesystem::path("graph_");

    for(int i = num_nodes; i <= max_edges; i += step_size){
        Graph *g = new Graph(num_nodes);
        std::string path = directory.string() + std::to_string(i) + ".graph";
        g -> generate_graph(seed, i);
        g -> save_graph(path);
        delete g;
    }
    return 0;
}