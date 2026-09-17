from matplotlib import pyplot as plt 
import matplotlib.patches as patches
import matplotlib.lines as lines
import networkx as netx 
import pathlib as pth


class Graph:
    def __init__(
        self, graph_path: pth.Path, 
        approx_path: pth.Path, 
        vertex_cover_path: pth.Path, 
        save_path: pth.Path
    ):
        with open(graph_path) as file:
            init_data = file.readline().split(" ")
            
            num_nodes = int(init_data[0])

            self.nodes: set[int] = set(range(num_nodes))
            self.edges: set[set[int]] = set()

            for line in file.readlines():
                edge = line.split(",")
                u, v = int(edge[0]), int(edge[1])
    
                if (u,v) in self.edges or (v,u) in self.edges:
                    continue
                self.edges.add((u,v))
        
        with open(approx_path) as file:
            init_data = file.readline().split(" ")
            
            self.approx_runtime = int(init_data[2])

            self.matching_edges: set[tuple[int,int]] = set()
            self.approx_vertex_cover: set[int] = set()

            for line in file.readlines():
                edge = line.split(" ")
                u, v = int(edge[0]), int(edge[1])
    
                if (u,v) in self.matching_edges or (v,u) in self.matching_edges:
                    continue
                self.matching_edges.add((u,v))
                self.approx_vertex_cover.update([u,v])
        
        with open(vertex_cover_path) as file:
            init_data = file.readline().split(" ")
            
            self.brute_force_runtime = int(init_data[0])

            self.vertex_cover: set[int] = {int(x) for x in file.readline().strip().split(" ")}

        self.save_path = save_path.joinpath(graph_path.stem + ".png")


    def get_data(self, ) -> dict:
        return {
            "numNodes" : len(self.nodes),
            "numEdges" : len(self.edges),
            "sizeVertexCover" : len(self.vertex_cover),
            "sizeMaximalMatching" : len(self.matching_edges),
            "sizeApproxVertexCover" : len(self.approx_vertex_cover),
            "runtimeVertexCover" : self.brute_force_runtime,
            "runtimeApproxVertexCover" : self.approx_runtime
        }



    def save_visualisation(self, ) -> None:
        graph = netx.Graph()
        figure, axes = plt.subplots(1,2, figsize=(12,6), layout="constrained")

        graph.add_nodes_from(self.nodes)
        graph.add_edges_from([tuple(x) for x in self.edges])

        pending_edges = self.edges - self.matching_edges
 
        all_edges = [tuple(x) for x in self.edges]
        pending_edges = [tuple(x) for x in pending_edges]
        matching_edges = [tuple(x) for x in self.matching_edges]

        avc_pending_nodes = self.nodes - self.approx_vertex_cover
        vc_pending_nodes = self.nodes - self.vertex_cover


        layout = netx.spring_layout(graph, k=2)

        netx.draw_networkx_nodes(graph, layout, avc_pending_nodes, node_color="blue", ax=axes[0])
        netx.draw_networkx_nodes(graph, layout, self.approx_vertex_cover, node_color="red", ax=axes[0])

        netx.draw_networkx_edges(graph, layout, pending_edges, edge_color="black", ax=axes[0], alpha=0.5, width=1)
        netx.draw_networkx_edges(graph, layout, matching_edges, edge_color="green", ax=axes[0], alpha=1, width=2)


        netx.draw_networkx_nodes(graph, layout, vc_pending_nodes, node_color="blue", ax=axes[1])
        netx.draw_networkx_nodes(graph, layout, self.vertex_cover, node_color="orange", ax=axes[1])

        netx.draw_networkx_edges(graph, layout, all_edges, edge_color="black", ax=axes[1], alpha=0.7)

        avc_node_handle = patches.Circle((0, 0), radius=0.1, color="red", label="Approx Vertex Cover")
        m_edge_handle = lines.Line2D([], [], color="green", linewidth=2, label="Matching Edge")

        axes[0].legend(handles=[avc_node_handle, m_edge_handle], loc="upper left", framealpha=0.3)

        vc_node_handle = patches.Circle((0, 0), radius=0.1, color="orange", label="Optimal Vertex Cover")

        axes[1].legend(handles=[vc_node_handle], loc="upper left", framealpha=0.3)

        figure.savefig(self.save_path, dpi=300)

        plt.close(figure)
