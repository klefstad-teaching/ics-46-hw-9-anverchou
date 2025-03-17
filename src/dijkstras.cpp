#include "dijkstras.h"

std::vector<int> dijkstra_shortest_path(const Graph & G, int source, std::vector<int> & previous) {
    int n = G.numVertices;
    std::vector<int> distance(n, INF);
    std::vector<bool> visited(n, false);

    for (int i = 0; i < n; i++) {
        previous[i] = -1;
    }

    distance[source] = 0;

    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<std::pair<int,int>>
    > pq;

    // Push the source with distance -
    pq.push(std::make_pair(0, source));

    // Dijstra loop
    while (!pq.empty()) {
        // Extract the vertex with the smallest distance
        int dist_u = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Skip if already visited
        if (visited[u]) {
            continue;
        }
        visited[u] = true;

        for (const Edge & edge : G[u]) {
            int v = edge.dst;
            int w = edge.weight;

            if (!visited[v] && dist_u + w < distance[v]) {
                distance[v] = dist_u + w;
                previous[v] = u;
                pq.push(std::make_pair(distance[v], v));
            }
        }
    }

    return distance;
}

// Extract Shortest Path
std::vector<int> extract_shortest_path(const std::vector<int> & distance, const std::vector<int> & previous, int destination) {
    if (destination < 0 || destination >= static_cast<int>(distance.size()) || distance[destination] == INF) {
        return {};
    }

    std::vector<int> path;
    for (int at = destination; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}


//Print Path
void print_path(const std::vector<int> & path, int total) {
    // Return if path is empty
    if (path.empty()) {
        std::cout << "\n";
        std::cout << "Total cost is " << total << "\n";
        return;
    }
    for (std::size_t i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i + 1 < path.size()) {
            std::cout << " ";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "\n";
    std::cout << "Total cost is " << total << "\n";
}


