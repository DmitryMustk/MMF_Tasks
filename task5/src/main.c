#include <stdio.h>

#define MAX_VERTICES 100
#define INFINITY 999999

int visited[MAX_VERTICES];
int graph[MAX_VERTICES][MAX_VERTICES];
int minCycleLength;

void initializeGraph() {
    int i, j;
    for (i = 0; i < MAX_VERTICES; i++) {
        visited[i] = 0;
        for (j = 0; j < MAX_VERTICES; j++) {
            graph[i][j] = 0;
        }
    }
}

void addEdge(int u, int v) {
    graph[u][v] = 1;
    graph[v][u] = 1;
}

void DFS(int v, int parent, int startVertex, int depth) {
    visited[v] = 1;

    int i;
    for (i = 0; i < MAX_VERTICES; i++) {
        if (graph[v][i]) {
            if (!visited[i]) {
                DFS(i, v, startVertex, depth + 1);
            } else if (i != parent && i == startVertex) {
                if (depth + 1 < minCycleLength) {
                    minCycleLength = depth + 1;
                }
                return;
            }
        }
    }
}

int findShortestCycle(int numVertices) {
    int i;
    minCycleLength = INFINITY;

    for (i = 0; i < numVertices; i++) {
        if (!visited[i]) {
            DFS(i, -1, i, 0);
        }
    }

    if (minCycleLength != INFINITY) {
        return minCycleLength;
    } else {
        return -1;  // No cycle found
    }
}

int main() {
    int numVertices, numEdges, i;
    printf("Enter the number of vertices: ");
    scanf("%d", &numVertices);

    printf("Enter the number of edges: ");
    scanf("%d", &numEdges);

    initializeGraph();

    printf("Enter the edges (vertex u, vertex v):\n");
    for (i = 0; i < numEdges; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        addEdge(u, v);
    }

    int shortestCycleLength = findShortestCycle(numVertices);

    if (shortestCycleLength == -1) {
        printf("Graph does not contain any cycles.\n");
    } else {
        printf("Length of the shortest cycle: %d\n", shortestCycleLength);
    }

    return 0;
}
