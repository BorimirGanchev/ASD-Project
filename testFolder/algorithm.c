#include <stdio.h>
#include "algorithm.h"

int graph[MAX][MAX];
int n;
int visited[MAX];  // Define this array

void initializeGraph() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            graph[i][j] = INT_MAX;
        }
    }
}

void addEdge(int u, int v, int w) {
    graph[u][v] = w;
    graph[v][u] = w;
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

    for (i = 0; i < j / 2; i++) {
        int temp = path[i];
        path[i] = path[j - i - 1];
        path[j - i - 1] = temp;
    }
    path[j] = -1;
}

void nearestNeighbor(int start, int *finalPath) {
    long long totalDistance = 0;
    int current = start;
    int path[MAX];
    int pathIndex = 0;
    int finalPathIndex = 0;

    for (int i = 0; i < MAX; i++) {  // Initialize visited array
        visited[i] = 0;
    }

    path[pathIndex++] = current;
    visited[current] = 1;
    finalPath[finalPathIndex++] = current;

    while (pathIndex < n) {
        int next = findNearestNeighbor(current);

        if (next == -1) {
            int minDist = INT_MAX;
            int nearestNode = -1;
            int shortestPath[MAX], distance;

            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    dijkstra(current, i, &distance, shortestPath);
                    if (distance < minDist) {
                        minDist = distance;
                        nearestNode = i;
                    }
                }
            }

            if (nearestNode == -1) {
                printf("\nNo more unvisited nodes reachable.\n");
                break;
            }

            dijkstra(current, nearestNode, &distance, shortestPath);
            for (int i = 1; shortestPath[i] != -1; i++) {
                totalDistance += graph[current][shortestPath[i]];
                visited[shortestPath[i]] = 1;
                current = shortestPath[i];
                path[pathIndex++] = current;
                finalPath[finalPathIndex++] = current;
            }
        } else {
            totalDistance += graph[current][next];
            visited[next] = 1;
            current = next;
            path[pathIndex++] = current;
            finalPath[finalPathIndex++] = current;
        }
    }

    int distance, shortestPath[MAX];
    dijkstra(current, start, &distance, shortestPath);

    if (distance != INT_MAX) {
        for (int i = 1; shortestPath[i] != -1; i++) {
            totalDistance += graph[current][shortestPath[i]];
            current = shortestPath[i];
            finalPath[finalPathIndex++] = current;
        }
    }

    finalPath[finalPathIndex] = -1;
}
