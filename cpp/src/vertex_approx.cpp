#include <graph.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>

Graph* load_graph_from_file(const std::filesystem::path& filename);
std::vector<Edge> find_maximal_matching(Graph *g);
void store_maximal_matching(std::vector<Edge>&, std::filesystem::path&, int, int, int);


Graph* load_graph_from_file(const std::filesystem::path& filename){
    Graph* g = new Graph();
    g->load_graph(filename.string());
    return g;
}


std::vector<Edge> find_maximal_matching(Graph *g){
    std::vector<Edge> edge_list = g -> get_edge_list();
    std::vector<Edge> maximal_matching;

    bool* covered = new bool[g -> numNodes]{0};

    for(auto edge : edge_list){
        int u = edge.node_u;
        int v = edge.node_v;

        if (covered[u] == true || covered[v] == true)
            continue;

        maximal_matching.emplace_back(edge);
        
        covered[u] = true;
        covered[v] = true;
    }

    delete[] covered;

    return maximal_matching;
}


void store_maximal_matching(std::vector<Edge>& matching, std::filesystem::path &path, int runtime, int n, int m){
    std::fstream file(path.string(), std::ios::out);
    file << n << " " << m << " " << runtime << "\n";
    for(auto edge : matching){
        int u = edge.node_u;
        int v = edge.node_v;
        file << u << " " << v << "\n";
    }
    file.close();
}


int main(int argc, char **argv){
    
    if(argc < 2){
        std::cout << "This program needs atleast 1 arguments\n./vertex_approx <Graph File> [Output Directory]" << std::endl;
        return 1;
    }

    std::filesystem::path source(argv[1]);
    std::filesystem::path out_dir(argv[2]);

    if(!std::filesystem::exists(out_dir)){
        std::filesystem::create_directory(out_dir);
    }

    std::filesystem::path out_file = out_dir / source.stem();
    out_file += ".maximal_matching";

    if(!std::filesystem::exists(source)){
        std::cout << "Please ensure path for Graph file is correct\n./vertex_approx <Graph File> [Output Directory]" << std::endl;
        return 1;
    }

    Graph* g = load_graph_from_file(source);

    std::chrono::high_resolution_clock clock;
    auto start = clock.now();

    auto edge_list = find_maximal_matching(g);

    auto end = clock.now();

    int duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    store_maximal_matching(edge_list, out_file, duration, g->numNodes, g->numEdges);

    delete g;
    return 0;
}