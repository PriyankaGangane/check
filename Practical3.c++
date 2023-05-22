#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>

#define NUM_THREADS 4

// Function to perform BFS on the graph
void bfs(int startVertex, const std::vector<std::vector<int>> &graph, std::vector<bool> &visited)
{
    std::queue<int> q;
    q.push(startVertex);
    visited[startVertex] = true;

    while (!q.empty())
    {
        int currentVertex = q.front();
        q.pop();
        std::cout << currentVertex << " ";

#pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < graph[currentVertex].size(); ++i)
        {
            int neighbor = graph[currentVertex][i];

// Acquire a lock to access visited array
#pragma omp critical
            {
                if (!visited[neighbor])
                {
                    q.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
    }
}

int main()
{
    // Create an undirected graph
    std::vector<std::vector<int>> graph = {
        {1, 2},    // Vertex 0 is connected to 1 and 2
        {0, 2, 3}, // Vertex 1 is connected to 0, 2, and 3
        {0, 1, 4}, // Vertex 2 is connected to 0, 1, and 4
        {1, 4, 5}, // Vertex 3 is connected to 1, 4, and 5
        {2, 3},    // Vertex 4 is connected to 2 and 3
        {3}        // Vertex 5 is connected to 3
    };

    // Number of vertices in the graph
    int numVertices = graph.size();

    // Create a visited array to track visited vertices
    std::vector<bool> visited(numVertices, false);

    // Perform BFS starting from vertex 0
    bfs(0, graph, visited);

    return 0;
}