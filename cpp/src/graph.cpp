#include <graph.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Graph::Graph(Graph &g){
    this->numNodes = g.numNodes;
    this->numEdges = g.numEdges;
    init_matrix();
    for (int i = 0; i < numNodes; ++i){
        for (int j = 0; j < numNodes; ++j)
        {
            matrix[i][j] = g.matrix[i][j];
        }
        
    }
}

void Graph::init_matrix(){
    this->matrix = new int*[numNodes];
    
    for (int i = 0; i < numNodes; ++i){
        this -> matrix[i] = new int[numNodes]{0};
    }
}

Graph::Graph(int num){
    this->numNodes = num;
    init_matrix();
}

void Graph::generate_graph(int seed, int numberOfEdges){
    srand(seed);
    int i = 0;
    this->numEdges = numberOfEdges;

    std::vector<Edge> all_edges;

    for(int u = 0; u < numNodes - 1; ++u){
        for(int v = u+1; v < numNodes; ++v){
            all_edges.emplace_back(u,v);
        }
    }

    shuffle(all_edges);

    for(int i = 0; i < numberOfEdges; ++i){
        int u = all_edges[i].node_u, v = all_edges[i].node_v;
        matrix[u][v] = 1;
        matrix[v][u] = 1;
    }

/*
    while (i < numberOfEdges){
        int u,v;
        u = rand() % numNodes;
        v = rand() % numNodes;
    
        if(u == v || matrix[u][v] == 1 || matrix[v][u] == 1)
            continue;
        
        matrix[u][v] = 1;
        matrix[v][u] = 1;
        ++i;
    }
*/
}

void Graph::load_graph(std::string filename){
    std::fstream file(filename, std::ios::in);
    std::string line;

    std::getline(file, line, ' ');
    numNodes = stoi(line);

    std::getline(file, line);
    numEdges = stoi(line);

    init_matrix();


    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string u_str, v_str;
        if(std::getline(ss, u_str, ',') && std::getline(ss, v_str)){
            int u = stoi(u_str);
            int v = stoi(v_str);

            matrix[u][v] = 1;
        }
    }

    file.close();
}

void Graph::save_graph(std::string filename){
    std::fstream file(filename, std::ios::out);
    file << numNodes << " " << numEdges << "\n";
    for(int u = 0; u < numNodes; ++u){
        for(int v = 0; v < numNodes; ++v){
            if (matrix[u][v] == 1){
                file << u << "," << v << "\n";
            }
        }
    }
    file.close();
}

void Graph::print_graph(){
    for(int u = 0; u < numNodes; ++u){
        for(int v = 0; v < numNodes; ++v){
            std::cout << matrix[u][v] << "\t";
        }
        std::cout << std::endl;
    }
}

Graph::~Graph(){
    for(int i = 0; i < numNodes; ++i){
        delete[] matrix[i];
    }
    delete[] matrix;
}

std::vector<Edge> Graph::get_edge_list(){
    std::vector<Edge> edges;
    for(int u = 0; u < numNodes; ++u){
        for(int v = 0; v < numNodes; ++v){
            if (matrix[u][v] > 0){
                edges.emplace_back(u, v);
            }
        }
    }
    return edges; 
}

void shuffle(std::vector<Edge> &edges){
    int size = edges.size();
    for(int i = 0; i < size; ++i){
        int a = rand() % size;
        int b = rand() % size;
        
        std::swap(edges[a], edges[b]);
    }
}