import pathlib as pth
import subprocess
from graph import Graph
import argparse
import csv
import tqdm


def parse() -> argparse.Namespace:
    parser = argparse.ArgumentParser()

    parser.add_argument("--executable_dir", required=True, type=str, help="Executable path of binary for finding approx path")
    parser.add_argument("--resources", required=True, type=str, help="Directory that has graphs, vertex, approx sub-directories")
    parser.add_argument("--seed", type=int, help="Seed for prng")
    parser.add_argument("--num_nodes", type=int, help="Number of nodes in randomly generated graphs")
    parser.add_argument("--step_size", type=int, help="Step size when generating edges.")

    return parser.parse_args()


def generate_graph(exec_path: pth.Path, output_dir: pth.Path, seed: int, num_nodes: int, step_size: int) -> None:
    args = [
        f"{exec_path.absolute()}",
        f"{seed}",
        f"{num_nodes}", 
        f"{step_size}", 
        f"{output_dir}"
    ]

    try: 
        process = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except Exception as err:
        print("Unable to generate graphs.", args,  err, sep="\n")


def find_optimal_vertex_cover(exec_path: pth.Path, graph_file: pth.Path, output_dir: pth.Path) -> None:
    args = [
        f"{exec_path}",
        f"{graph_file}",
        f"{output_dir}"
    ]

    try: 
        process = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except Exception as err:
        print("Unable to find optimal vertex cover.", args,  err, sep="\n")


def find_approx_vertex_cover(exec_path: pth.Path, graph_file: pth.Path, output_dir: pth.Path) -> None:
    args = [
        f"{exec_path}",
        f"{graph_file}",
        f"{output_dir}"
    ]

    try: 
        process = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except Exception as err:
        print("Unable to find approximate vertex cover.", args,  err, sep="\n")


def visualise_and_get_data(graph_file: pth.Path, vertex_cover_path: pth.Path, approx_path: pth.Path, output_dir: pth.Path) -> dict:
    g = Graph(graph_file, approx_path, vertex_cover_path, output_dir)
    g.save_visualisation()
    return g.get_data()


def main():
    opts = parse()

    res = pth.Path(opts.resources)
    executable_dir = pth.Path(opts.executable_dir)

    graph_generator_exec = executable_dir.joinpath("graph_generator")
    vertex_exec = executable_dir.joinpath("vertex")
    vertex_approx_exec = executable_dir.joinpath("vertex_approx")


    graph_dir = res.joinpath("graphs")
    vertex_dir = res.joinpath("vertex")
    approx_dir = res.joinpath("approx")
    output_dir = res.joinpath("images")

    graph_dir.mkdir(exist_ok=True, parents=True)
    vertex_dir.mkdir(exist_ok=True)
    approx_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)

    if len(list(res.walk())) != 5:
        print("Please provide an empty directory as --resources")
        return

    generate_graph(graph_generator_exec, graph_dir, opts.seed, opts.num_nodes, opts.step_size)
    print("Generated Graphs")

    graphs = list(graph_dir.glob("*.graph"))
    graphs = sorted(graphs, key = lambda x: int(x.stem.split("_")[1]))

    if not output_dir.exists():
        output_dir.mkdir()

    data_rows = []

    for files in tqdm.cli.tqdm(graphs):
        find_optimal_vertex_cover(vertex_exec, files, vertex_dir)
        find_approx_vertex_cover(vertex_approx_exec, files, approx_dir)

        vertex_file = vertex_dir.joinpath(files.stem + ".vertex_cover")
        approx_file = approx_dir.joinpath(files.stem + ".maximal_matching")
        
        data = visualise_and_get_data(
            files,
            vertex_file, 
            approx_file, 
            output_dir
        )

        af = data['sizeApproxVertexCover'] / data['sizeVertexCover']

        data_r = [
            f"{data['numNodes']},{data['numEdges']}",
            f"{data['sizeVertexCover']}",
            f"{data['runtimeVertexCover'] * 10e-6:0.8f}",
            f"{data['sizeApproxVertexCover']}",
            f"{af:0.3f}",
            f"{data['runtimeApproxVertexCover'] * 10e-6:0.8f}"
        ]

        data_rows.append(data_r)

    header_row1 = ['(n, m)', 'P1', '', 'P2', '', '']
    header_row2 = ['', 'BF', '', 'AA', '', '']
    header_row3 = ['', 'Size', 'Time (in seconds)', 'Size', 'AF', 'Time (in seconds)']

    csv_file = res.joinpath("table_data.csv")

    with open(csv_file, 'w') as file:
        table = csv.writer(file)
        table.writerows([header_row1, header_row2, header_row3])
        table.writerows(data_rows)


if __name__ == "__main__":
    main()