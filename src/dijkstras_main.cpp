#include "dijkstras.h"   
#include <iostream>
#include <string>
#include <vector>

static std::string get_arg(int argc, char* argv[], const std::string& default_value) {
    if (argc > 1) {
        return argv[1];
    }
    return default_value;
}

int main(int argc, char* argv[])
{
    // Choose file from command-line or fallback
    std::string filename = get_arg(argc, argv, "src/large.txt");

    Graph G;
    file_to_graph(filename, G);

    std::vector<int> previous(G.numVertices, -1);
    std::vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    // Print each path
    for (int i = 0; i < G.numVertices; ++i) {
        std::vector<int> path = extract_shortest_path(distances, previous, i);

        print_path(path, distances[i]);
        std::cout << "\n";
    }
    return 0;
}
