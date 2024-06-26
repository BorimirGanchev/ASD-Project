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

    int dist[MAX], prev[MAX], selected[MAX] = {0};
    int inf = INT_MAX, min, m, startNode = start, i, j;
    int firstDirection = -1;
    int firstDirectionSet = 0;

    for (i = 0; i < n; i++) {
        dist[i] = inf;
        prev[i] = -1;
    }

    dist[startNode] = 0;

    //iterate trough all nodes so it can find the one with the minimum distance
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

        //for each neigbour to the selected
        for (j = 0; j < n; j++) {
            if (!selected[j] && graph[m][j] != inf) {
                if (direction[m][j] == oppositeDirection(lastDir)) {
                    printf("Skipping edge %d -> %d due to opposite direction\n", m, j);
                } else {
                    int newDist = dist[m] + graph[m][j];
                    if (newDist < dist[j]) {
                        dist[j] = newDist;
                        prev[j] = m;
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
    //this is for no path and some switch fo debuging
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
        }
        return 0; 
    }

    i = end;
    j = 0;
    //reverse the path
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

    //this is for me
    if (firstDirectionSet) {
        const char* dirStr;
        switch (firstDirection) {
            case EAST: dirStr = "EAST"; break;
            case WEST: dirStr = "WEST"; break;
            case NORTH: dirStr = "NORTH"; break;
            case SOUTH: dirStr = "SOUTH"; break;
            default: dirStr = "UNKNOWN"; break;
        }
    }

    return 1; 
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

        int next = findNearestNeighbor(current, lastDir);

        //this is for the blockage
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

            //this is if all nodes are found
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

        //check if everyting is visited
        int allVisited = 1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                allVisited = 0;
                break;
            }
        }
        if (allVisited) break;
    }

    //block for the path to pack
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
    
    int startNode = 0;
    int pathLength;
    int* path = nearestNeighbor(startNode, &pathLength);

    FILE *fptr;
    fptr = fopen("filename.txt", "w");

    for (int i = 0; i < pathLength + 1; i++) {
        fprintf(fptr, "%d, ", path[i]);
    }
    printf("\n");

    fclose(fptr);

    return 0;
}
