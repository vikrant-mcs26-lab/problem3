#include <graph.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <chrono>

std::unordered_set<int> find_vertex_cover(Graph *g);
bool check_vertex_cover(std::vector<Edge> edge_list, std::unordered_set<int> &vertex_cover);
inline bool contains(std::unordered_set<int> &vertex_cover, int value);
Graph* initialise_graph();
void save_graph(std::filesystem::path, std::unordered_set<int>, int time, Graph &g);

std::unordered_set<int> find_vertex_cover(Graph *g){
    std::vector<Edge> edge_list = g -> get_edge_list();
    std::unordered_set<int> vertex_covers;

    int minValue = g->numNodes + 1;

    for(int i = 0; i < (1 << g->numNodes); ++i){
        std::unordered_set<int> subset;
        for(int j = 0; j <= g->numNodes; ++j){
            if ((i & (1 << j)) > 0){
                subset.insert(j);
            }
        }
        if (subset.size() < minValue && check_vertex_cover(edge_list, subset)){
            vertex_covers = subset;
            minValue = subset.size();
        }
    }

    return vertex_covers;
}

bool check_vertex_cover(std::vector<Edge> edge_list, std::unordered_set<int> &vertex_cover){
    for(auto edge: edge_list){
        if(!contains(vertex_cover, edge.node_u) && !contains(vertex_cover, edge.node_v))
            return false;
    }
    return true;
}

inline bool contains(std::unordered_set<int> &vertex_cover, int value){
    return vertex_cover.find(value) != vertex_cover.end();
}

void save_graph(std::filesystem::path filename, std::unordered_set<int> vector_set, int time, Graph& g){
    std::fstream file(filename, std::ios::out);

    file << time << " " << g.numNodes << " " << g.numEdges << "\n";

    for (auto item : vector_set){
        file << item << " ";
    }
    file << "\n";
    
    file.close();
}

int main(int argc, char **argv){
    if(argc < 2){
        std::cout << "This program needs atleast 1 arguments\n./vertex_cover <Graph File> [Output File]" << std::endl;
        return 1;
    }
    std::filesystem::path source(argv[1]);

    if(!std::filesystem::exists(source)){
        std::cout << "Please ensure path for Graph file is correct\n./vertex_cover <Graph File> [Output Directory]" << std::endl;
        return 1;
    }

    
    Graph g;
    g.load_graph(source.string());
    
    auto start_time = std::chrono::high_resolution_clock::now();

    std::unordered_set<int> vertex_cover = find_vertex_cover(&g);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();


    if(argc == 3){
        std::filesystem::path dest_folder(argv[2]);
        std::filesystem::path save_file_name = dest_folder / source.stem();
        save_file_name += ".vertex_cover";
        if(!std::filesystem::is_directory(dest_folder)){
            std::cout << "Please ensure path for Destination path is correct\n./vertex_cover <Graph File> [Output Directory]" << std::endl;
            return 1;
        }
        save_graph(save_file_name, vertex_cover, total_time, g);
    }

    return 0;
}