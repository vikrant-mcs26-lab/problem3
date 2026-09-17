#include <vector>
#include <string>

#ifndef GRAPH_HPP
#define GRAPH_HPP

typedef struct Edge{
    int node_u;
    int node_v;

    Edge(int u, int v) : node_u(u), node_v(v) { }
    Edge() : node_u(-1), node_v(-1) { }
} Edge;

void shuffle(std::vector<Edge> &);

class Graph {

private:
    void init_matrix();

public:
    int numNodes;
    int numEdges;
    int** matrix;

    Graph() {}
    Graph(Graph &g);
    Graph(int num);
    ~Graph();

    void load_graph(std::string filename);
    void save_graph(std::string filename);
    void generate_graph(int seed, int numberOfEdges);

    std::vector<Edge> get_edge_list();

    void print_graph();
};

#endif