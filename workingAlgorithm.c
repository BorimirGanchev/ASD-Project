#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100 // Define the maximum number of nodes

int graph[MAX][MAX];
int visited[MAX];
int n; // Number of nodes

void initializeGraph() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            graph[i][j] = INT_MAX; // Initialize all distances to infinity
        }
        visited[i] = 0; // Initialize all nodes as unvisited
    }
}

void addEdge(int u, int v, int w) {
    graph[u][v] = w; // Add edge u -> v with weight w
    graph[v][u] = w; // Add edge v -> u with weight w (making it bidirectional)
}

int findNearestNeighbor(int current) {
    int nearest = -1;
    int minDist = INT_MAX;
    
    for (int i = 0; i < n; i++) {
        if (!visited[i] && graph[current][i] < minDist) {
            minDist = graph[current][i];
            nearest = i;
        }
    }
    
    return nearest;
}

void dijkstra(int start, int end, int *distance, int *path) {
    int dist[MAX], prev[MAX], selected[MAX] = {0};
    int inf = INT_MAX, min, m, startNode = start, i, j;
    
    for (i = 0; i < n; i++) {
        dist[i] = inf;
        prev[i] = -1;
    }
    
    dist[startNode] = 0;
    
    for (i = 0; i < n; i++) {
        min = inf;
        for (j = 0; j < n; j++) {
            if (!selected[j] && dist[j] < min) {
                min = dist[j];
                m = j;
            }
        }
        
        selected[m] = 1;
        
        for (j = 0; j < n; j++) {
            if (!selected[j] && graph[m][j] != inf && dist[m] + graph[m][j] < dist[j]) {
                dist[j] = dist[m] + graph[m][j];
                prev[j] = m;
            }
        }
    }
    
    *distance = dist[end];
    i = end;
    j = 0;
    while (i != -1) {
        path[j++] = i;
        i = prev[i];
    }
    
    // Reverse the path array
    for (i = 0; i < j / 2; i++) {
        int temp = path[i];
        path[i] = path[j - i - 1];
        path[j - i - 1] = temp;
    }
    path[j] = -1; // Mark the end of the path with -1
}

void nearestNeighbor(int start) {
    long long totalDistance = 0;
    int current = start;
    int path[MAX];
    int pathIndex = 0;
    int backtrackStack[MAX];
    int backtrackStackIndex = 0;

    path[pathIndex++] = current;
    visited[current] = 1;

    printf("Path: %d", current);

    while (pathIndex < n) {
        int next = findNearestNeighbor(current);

        if (next == -1) {
            // If no unvisited neighbor is found, backtrack
            if (backtrackStackIndex == 0) {
                printf("\nNo path to continue.\n");
                break;
            }
            current = backtrackStack[--backtrackStackIndex];
            printf(" (backtrack) -> %d", current);
            continue;
        }

        printf(" -> %d", next);
        totalDistance += graph[current][next];
        visited[next] = 1;
        current = next;
        path[pathIndex++] = current;
        backtrackStack[backtrackStackIndex++] = current; // Push current to backtrack stack
    }
    
    if (pathIndex == n) {
        // Find the shortest path back to the start node
        int distance, shortestPath[MAX];
        dijkstra(current, start, &distance, shortestPath);
        
        if (distance != INT_MAX) {
            for (int i = 1; shortestPath[i] != -1; i++) {
                printf(" -> %d", shortestPath[i]);
                totalDistance += graph[shortestPath[i-1]][shortestPath[i]];
            }
            printf("\nTotal distance: %lld\n", totalDistance);
            return; // Terminate the program after returning to the start node
        } else {
            printf("\nNo path to return to the start node.\n");
        }
    }
    
    printf("\nTotal distance: %lld\n", totalDistance);
}

int main() {
    initializeGraph();

    n = 48; // Define the number of nodes
    // Add bidirectional edges using addEdge(u, v, w)
    addEdge(0, 1, 1);
    addEdge(1, 2, 2);
    addEdge(2, 3, 2);
    addEdge(3, 4, 2);
    addEdge(4, 6, 3);
    addEdge(6, 8, 3);
    addEdge(8, 10, 2);
    addEdge(10, 12, 1);
    addEdge(12, 14, 1);
    addEdge(14, 16, 1);
    addEdge(16, 17, 1); 
    addEdge(17, 18, 1); 
    addEdge(18, 15, 2); 
    addEdge(15, 14, 3); 
    addEdge(14, 13, 1); 
    addEdge(13, 15, 3); 
    addEdge(11, 13, 2); 
    addEdge(8, 11, 3); 
    addEdge(8, 9, 2); 
    addEdge(9, 11, 1); 
    addEdge(9, 7, 1); 
    addEdge(7, 5, 1); 
    addEdge(5, 45, 2); 
    addEdge(1, 45, 2); 
    addEdge(0, 46, 2); 
    addEdge(46, 45, 1); 
    addEdge(45, 44, 1); 
    addEdge(44, 38, 1); 
    addEdge(7, 38, 1); 
    addEdge(9, 37, 2); 
    addEdge(37, 38, 3); 
    addEdge(37, 36, 3);
    addEdge(36, 35, 3); 
    addEdge(38, 35, 3); 
    addEdge(36, 15, 1); 
    addEdge(15, 19, 1); 
    addEdge(18, 19, 1); 
    addEdge(18, 20, 2); 
    addEdge(35, 19, 1); 
    addEdge(19, 20, 2); 
    addEdge(19, 22, 2); 
    addEdge(20, 22, 2); 
    addEdge(19, 21, 1); 
    addEdge(20, 23, 2); 
    addEdge(23, 25, 2); 
    addEdge(22, 25, 1); 
    addEdge(22, 24, 3); 
    addEdge(24, 25, 3); 
    addEdge(21, 24, 3); 
    addEdge(25, 26, 3); 
    addEdge(26, 27, 3); 
    addEdge(27, 28, 1); 
    addEdge(28, 29, 1); 
    addEdge(26, 29, 1); 
    addEdge(24, 29, 1); 
    addEdge(24, 33, 1); 
    addEdge(33, 29, 1); 
    addEdge(29, 30, 2); 
    addEdge(30, 31, 2); 
    addEdge(31, 41, 2); 
    addEdge(41, 42, 1); 
    addEdge(42, 43, 1); 
    addEdge(21, 35, 1); 
    addEdge(21, 34, 2); 
    addEdge(34, 33, 2); 
    addEdge(33, 32, 2); 
    addEdge(32, 39, 2); 
    addEdge(33, 39, 1); 
    addEdge(34, 40, 2); 
    addEdge(40, 39, 2); 
    addEdge(44, 40, 2); 
    addEdge(39, 43, 2); 
    addEdge(43, 47, 2); 
    addEdge(47, 46, 2); 
    addEdge(44, 47, 1);

    int startNode = 0; // Define the start node
    nearestNeighbor(startNode);
    
    return 0;
}
