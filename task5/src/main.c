#include <stdio.h>

#define MAX_VERTICES 5000
#define INFINITY 999999

int visited[MAX_VERTICES];
int graph[MAX_VERTICES][MAX_VERTICES];
int minCycleLength;

typedef enum error_code {
    no_error = 0,
    other_error,
    bad_number_of_lines,
    bad_number_of_vertices,
    bad_number_of_edges,
    bad_vertex,
} error_code;

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

int readInput(int* numVertices, int* numEdges, const char* in_stream) {
    FILE* fp = fopen(in_stream, "r");
    if(!fp)
        return other_error;
    if (fscanf(fp, "%d", numVertices) != 1) {
        fclose(fp);
        return bad_number_of_lines;
    }
    if (*numVertices < 0 || *numVertices > MAX_VERTICES) {
        fclose(fp);
        return bad_number_of_vertices;
    }
    if (fscanf(fp, "%d", numEdges) != 1) {
        fclose(fp);
        return bad_number_of_lines;
    }
    if (*numEdges < 0 || *numEdges > *numVertices * (*numVertices - 1) / 2) {
        fclose(fp);
        return bad_number_of_edges;
    }
    int from, to;
    for (int i = 0; i < *numEdges; ++i) {
        if (fscanf(fp, "%d %d", &from, &to) != 2) {
            fclose(fp);
            return bad_number_of_lines;
        }
        if (from < 0 || to < 0 || from > *numVertices || to > *numVertices) {
            fclose(fp);
            return bad_vertex;
        }
        addEdge(from, to);
    }
    fclose(fp);
    return no_error;
}

int printError(error_code error, const char *out_stream) {
    FILE *fp = fopen(out_stream, "w");
    if (!fp)
        return other_error;
    if (error == bad_number_of_lines) {
        fprintf(fp, "bad number of lines");
    }
    if (error == bad_number_of_vertices) {
        fprintf(fp, "bad number of vertices");
    }
    if (error == bad_number_of_edges) {
        fprintf(fp, "bad number of edges");
    }
    if (error == bad_vertex) {
        fprintf(fp, "bad vertex");
    }
    fclose(fp);
    return no_error;
}


int main() {
    int numVertices, numEdges;
    const char* in_stream = "in.txt", *out_stream = "out.txt";
    initializeGraph();

    error_code error = readInput(&numVertices, &numEdges, in_stream);
    if(error != no_error){
        printError(error, out_stream);
        return 0;
    }
    int shortestCycleLength = findShortestCycle(numVertices);

    if (shortestCycleLength == -1) {
        printf("Graph does not contain any cycles.\n");
    } else {
        printf("Length of the shortest cycle: %d\n", shortestCycleLength);
    }

    return 0;
}
