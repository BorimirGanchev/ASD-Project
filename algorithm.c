#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "./constants.h"

int graph[MAX][MAX];
int direction[MAX][MAX];
int visited[MAX];
int n;

void initializeGraph() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            graph[i][j] = INT_MAX;
            direction[i][j] = 0;
        }
        visited[i] = 0;
    }
}

void addEdge(int u, int v, int w, int dir) {
    graph[u][v] = w;
    direction[u][v] = dir;
}

int oppositeDirection(int dir) {
    if (dir == NORTH) return SOUTH;
    if (dir == SOUTH) return NORTH;
    if (dir == EAST) return WEST;
    if (dir == WEST) return EAST;
    return 0;
}

int findNearestNeighbor(int current, int lastDir) {
    int nearest = -1;
    int minDist = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (!visited[i] && graph[current][i] < minDist && direction[current][i] != oppositeDirection(lastDir)) {
            minDist = graph[current][i];
            nearest = i;
        }
    }

    return nearest;
}

int dijkstra(int start, int end, int lastDir, int *distance, int *path) {
    printf("Dijkstra called with start: %d, end: %d, lastDir: %d\n", start, end, lastDir); // Print call info

    int dist[MAX], prev[MAX], selected[MAX] = {0};
    int inf = INT_MAX, min, m, startNode = start, i, j;
    int firstDirection = -1;
    int firstDirectionSet = 0;

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

        if (min == inf) break;

        selected[m] = 1;

        for (j = 0; j < n; j++) {
            if (!selected[j] && graph[m][j] != inf) {
                if (direction[m][j] == oppositeDirection(lastDir)) {
                    printf("Skipping edge %d -> %d due to opposite direction\n", m, j);
                } else {
                    int newDist = dist[m] + graph[m][j];
                    if (newDist < dist[j]) {
                        dist[j] = newDist;
                        prev[j] = m;
                        printf("Updated distance for node %d to %d with distance %d\n", j, newDist, lastDir);
                        if (!firstDirectionSet) {
                            firstDirection = direction[start][j];
                            firstDirectionSet = 1;
                        }
                    }
                }
            }
        }

        if (prev[m] != -1) {
            lastDir = direction[prev[m]][m];
        }
    }

    *distance = dist[end];
    if (dist[end] == inf) {
        if (firstDirectionSet) {
            const char* dirStr;
            switch (firstDirection) {
                case EAST: dirStr = "EAST"; break;
                case WEST: dirStr = "WEST"; break;
                case NORTH: dirStr = "NORTH"; break;
                case SOUTH: dirStr = "SOUTH"; break;
                default: dirStr = "UNKNOWN"; break;
            }
            printf("\n\n\nFirst direction taken: %s\n", dirStr);
        }
        printf("Dijkstra execution failed: No path found from %d to %d\n", start, end);
        printf("Partial path: ");
        i = start;
        while (i != -1) {
            printf("%d ", i);
            if (i == prev[i]) break; 
            i = prev[i];
        }
        printf("\n");
        return 0; 
    }

    i = end;
    j = 0;
    while (i != -1) {
        path[j++] = i;
        i = prev[i];
    }
    j--;

    for (i = 0; i < j / 2; i++) {
        int temp = path[i];
        path[i] = path[j - i - 1];
        path[j - i - 1] = temp;
    }
    path[j] = -1;

    printf("Dijkstra execution successful: Path found from %d to %d\n", start, end);
    printf("Path: ");
    for (i = 0; path[i] != -1; i++) {
        printf("%d ", path[i]);
    }
    printf("\nTotal distance: %d\n", dist[end]);

    if (firstDirectionSet) {
        const char* dirStr;
        switch (firstDirection) {
            case EAST: dirStr = "EAST"; break;
            case WEST: dirStr = "WEST"; break;
            case NORTH: dirStr = "NORTH"; break;
            case SOUTH: dirStr = "SOUTH"; break;
            default: dirStr = "UNKNOWN"; break;
        }
        printf("\n\n\nFirst direction taken: %s\n", dirStr);
    }

    return 1; 
}


void printVisitedUnvisited() {
    printf("Visited nodes: ");
    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            printf("%d ", i);
        }
    }
    printf("\nUnvisited nodes: ");
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void printDirection(int from, int to, int dir) {
    const char* dirStr;
    switch (dir) {
        case EAST: dirStr = "EAST"; break;
        case WEST: dirStr = "WEST"; break;
        case NORTH: dirStr = "NORTH"; break;
        case SOUTH: dirStr = "SOUTH"; break;
        default: dirStr = "UNKNOWN"; break;
    }
    printf("Moving from %d to %d in direction %s\n", from, to, dirStr);
}

int* nearestNeighbor(int start, int* pathLength) {
    static int path[MAX];
    int pathIndex = 0;
    int totalDistance = 0;
    int current = start;
    int lastDir = 0;

    path[pathIndex++] = current;
    visited[current] = 1;

    while (1) {
        printVisitedUnvisited();

        int next = findNearestNeighbor(current, lastDir);

        if (next == -1) {
            int minDist = INT_MAX;
            int nearestNode = -1;
            int shortestPath[MAX], distance;

            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    if (dijkstra(current, i, lastDir, &distance, shortestPath) && distance < minDist) {
                        minDist = distance;
                        nearestNode = i;
                    }
                }
            }

            if (nearestNode == -1) {
                break;
            }

            if (dijkstra(current, nearestNode, lastDir, &distance, shortestPath)) {
                for (int i = 0; shortestPath[i] != -1; i++) {
                    totalDistance += graph[current][shortestPath[i]];
                    lastDir = direction[current][shortestPath[i]];
                    visited[shortestPath[i]] = 1;
                    printDirection(current, shortestPath[i], lastDir); 
                    current = shortestPath[i];
                    path[pathIndex++] = current;
                }
            }
        } else {
            totalDistance += graph[current][next];
            lastDir = direction[current][next];
            visited[next] = 1;
            printDirection(current, next, lastDir);
            current = next;
            path[pathIndex++] = current;
        }

        int allVisited = 1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                allVisited = 0;
                break;
            }
        }
        if (allVisited) break;
    }

    int distance, shortestPath[MAX];
    if (dijkstra(current, start, lastDir, &distance, shortestPath)) {
        for (int i = 1; shortestPath[i] != -1; i++) {
            totalDistance += graph[current][shortestPath[i]];
            current = shortestPath[i];
            path[pathIndex++] = current;
        }
    } else {
        printf("No path back to start node found.\n");
    }

    *pathLength = pathIndex;
    return path;
}

int main() {
    initializeGraph();

    n = 22;
    addEdge(0, 1, 1, NORTH); addEdge(1, 0, 1, SOUTH);
    addEdge(1, 2, 1, NORTH); addEdge(2, 1, 1, SOUTH);
    addEdge(2, 3, 2, EAST); addEdge(3, 2, 2, SOUTH);
    addEdge(3, 4, 2, EAST); addEdge(4, 3, 2, WEST);
    addEdge(4, 5, 1, SOUTH); addEdge(5, 4, 1, NORTH);
    addEdge(5, 19, 2, WEST); addEdge(19, 5, 2, EAST);
    addEdge(19, 1, 2, NORTH); addEdge(1, 19, 2, SOUTH);
    addEdge(3, 6, 1, EAST); addEdge(6, 3, 1, WEST);
    addEdge(19, 7, 1, EAST); addEdge(7, 19, 1, WEST);
    addEdge(4, 6, 2, EAST); addEdge(6, 4, 2, WEST);
    addEdge(5, 7, 2, EAST); addEdge(7, 5, 2, WEST);
    addEdge(6, 10, 1, EAST); addEdge(10, 6, 1, WEST);
    addEdge(6, 8, 2, EAST); addEdge(8, 6, 2, WEST);
    addEdge(7, 20, 1, EAST); addEdge(20, 7, 1, WEST);
    addEdge(7, 9, 2, EAST); addEdge(9, 7, 2, WEST);
    addEdge(8, 9, 1, SOUTH); addEdge(9, 8, 1, NORTH);
    addEdge(8, 10, 2, EAST); addEdge(10, 8, 2, WEST);
    addEdge(8, 11, 2, SOUTH); addEdge(11, 8, 2, NORTH);
    addEdge(9, 11, 2, NORTH); addEdge(11, 9, 2, SOUTH);
    addEdge(9, 20, 2, EAST); addEdge(20, 9, 2, WEST);
    addEdge(10, 12, 2, EAST); addEdge(12, 10, 2, WEST);
    addEdge(11, 13, 2, EAST); addEdge(13, 11, 2, WEST);
    addEdge(20, 21, 2, EAST); addEdge(21, 20, 2, WEST);
    addEdge(12, 16, 3, EAST); addEdge(16, 12, 3, WEST);
    addEdge(21, 18, 2, EAST); addEdge(18, 21, 2, WEST);
    addEdge(12, 14, 2, EAST); addEdge(14, 12, 2, WEST);
    addEdge(13, 14, 2, NORTH); addEdge(14, 13, 2, SOUTH);
    addEdge(13, 15, 2, SOUTH); addEdge(15, 13, 2, NORTH);
    addEdge(21, 15, 2, EAST); addEdge(15, 21, 2, WEST);
    addEdge(15, 18, 2, EAST); addEdge(18, 15, 2, WEST);
    addEdge(14, 15, 1, SOUTH); addEdge(15, 14, 1, NORTH);
    addEdge(14, 16, 2, EAST); addEdge(16, 14, 2, WEST);
    addEdge(16, 17, 3, SOUTH); addEdge(17, 16, 3, WEST);
    addEdge(18, 17, 3, NORTH); addEdge(17, 18, 3, WEST);
    addEdge(19, 0, 1, SOUTH); addEdge(0, 19, 1, NORTH);
    // addEdge(17, 23, 3, SOUTH); addEdge(23, 17, 3, NORTH);
    // addEdge(1, 46, 2, SOUTH); addEdge(46, 1, 2, NORTH);
    // addEdge(0, 46, 1, SOUTH); addEdge(46, 0, 1, NORTH);
    // addEdge(46, 38, 2, NORTH); addEdge(38, 46, 2, WEST);
    // addEdge(46, 44, 3, NORTH); addEdge(44, 46, 3, NORTH);
    // addEdge(44, 38, 2, NORTH); addEdge(38, 44, 2, WEST);
    // addEdge(44, 40, 2, SOUTH); addEdge(40, 44, 2, WEST);
    // addEdge(46, 47, 1, SOUTH); addEdge(47, 46, 1, NORTH);
    // addEdge(47, 42, 1, SOUTH); addEdge(42, 47, 1, NORTH);
    // addEdge(47, 43, 2, SOUTH); addEdge(43, 47, 2, WEST);
    // addEdge(42, 43, 2, NORTH); addEdge(43, 42, 2, WEST);
    // addEdge(42, 41, 2, SOUTH); addEdge(41, 42, 2, WEST);
    // addEdge(43, 32, 2, EAST); addEdge(32, 43, 2, WEST);////////
    // addEdge(41, 31, 2, EAST); addEdge(31, 41, 2, WEST);
    // addEdge(31, 30, 2, EAST); addEdge(30, 31, 2, WEST);///////
    // addEdge(32, 33, 1, EAST); addEdge(33, 32, 1, WEST);
    // addEdge(40, 32, 2, EAST); addEdge(32, 40, 2, NORTH);
    // addEdge(32, 34, 2, NORTH); addEdge(34, 32, 2, WEST);
    // addEdge(40, 34, 2, EAST); addEdge(34, 40, 2, WEST);
    // addEdge(38, 35, 4, EAST); addEdge(35, 38, 4, WEST);
    // addEdge(35, 19, 1, EAST); addEdge(19, 35, 1, WEST);
    // addEdge(35, 21, 2, EAST); addEdge(21, 35, 2, NORTH);
    // addEdge(19, 21, 2, WEST); addEdge(21, 19, 2, NORTH);
    // addEdge(34, 21, 1, EAST); addEdge(21, 34, 1, WEST);
    // addEdge(21, 33, 2, SOUTH); addEdge(33, 21, 2, EAST);
    // addEdge(21, 24, 2, SOUTH); addEdge(24, 21, 2, WEST);//////
    // addEdge(33, 24, 1, EAST); addEdge(24, 33, 1, WEST);
    // addEdge(21, 29, 2, SOUTH); addEdge(29, 21, 2, NORTH);
    // addEdge(33, 29, 2, EAST); addEdge(29, 33, 2, NORTH);
    // addEdge(24, 29, 2, WEST); addEdge(29, 24, 2, NORTH);
    // addEdge(30, 29, 2, EAST); addEdge(29, 30, 2, SOUTH);
    // addEdge(30, 28, 1, EAST); addEdge(28, 30, 1, WEST);
    // addEdge(28, 29, 2, WEST); addEdge(29, 28, 2, SOUTH);
    // addEdge(19, 20, 1, EAST); addEdge(20, 19, 1, WEST);
    // addEdge(19, 22, 2, EAST); addEdge(22, 19, 2, NORTH);
    // addEdge(20, 22, 2, WEST); addEdge(22, 20, 2, NORTH);
    // addEdge(24, 22, 2, EAST); addEdge(22, 24, 2, SOUTH);
    // addEdge(22, 25, 2, SOUTH); addEdge(25, 22, 2, WEST);
    // addEdge(24, 25, 1, EAST); addEdge(25, 24, 1, WEST);
    // addEdge(28, 27, 2, EAST); addEdge(27, 28, 2, SOUTH);
    // addEdge(25, 27, 2, EAST); addEdge(27, 25, 2, NORTH);
    // addEdge(27, 23, 1, NORTH); addEdge(23, 27, 1, SOUTH);
    // addEdge(20, 23, 2, EAST); addEdge(23, 20, 2, NORTH);
    // int distance;
    // int path[MAX];
    // if (dijkstra(21, 9, 1, &distance, path)) {
    //     printf("Shortest path from 21 to 9 found:\n");
    //     for (int i = 0; path[i] != -1; i++) {
    //         printf("%d ", path[i]);
    //     }
    //     printf("\nTotal distance: %d\n", distance);
    // } else {
    //     printf("No path found from 21 to 9\n");
    // }



    int startNode = 0;
    int pathLength;
    int* path = nearestNeighbor(startNode, &pathLength);

    FILE *fptr;
    fptr = fopen("filename.txt", "w");

    printf("Nearest Neighbor Path: ");
    for (int i = 0; i < pathLength + 1; i++) {
        fprintf(fptr, "%d, ", path[i]);
    }
    printf("\n");

    fclose(fptr);

    return 0;
}
