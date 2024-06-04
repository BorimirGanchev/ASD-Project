#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <limits.h>

#define MAX 100

extern int graph[MAX][MAX];
extern int n;
extern int visited[MAX];  // Ensure this line is present

void initializeGraph();
void addEdge(int u, int v, int w);
int findNearestNeighbor(int current);
void dijkstra(int start, int end, int *distance, int *path);
void nearestNeighbor(int start, int *finalPath);

#endif
