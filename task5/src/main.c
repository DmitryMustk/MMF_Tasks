#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 5000
#define INFINITY INT_MAX

typedef enum error_code {
    no_error = 0,
    other_error,
    bad_number_of_lines,
    bad_number_of_vertices,
    bad_number_of_edges,
    bad_vertex,
} error_code;

typedef struct {
    int** graph;
    int* visited;
    int minCycleLength;
    int numVertices;
    int numEdges;
} graph_t;

void initializeGraph(graph_t* graph, int numVertices) {
    int i, j;
    graph->visited = (int*)malloc(numVertices * sizeof(int));
    graph->graph = (int**)malloc(numVertices * sizeof(int*));

    for (i = 0; i < numVertices; i++) {
        graph->visited[i] = 0;
        graph->graph[i] = (int*)malloc(numVertices * sizeof(int));

        for (j = 0; j < numVertices; j++) {
            graph->graph[i][j] = 0;
        }
    }
}

void addEdge(graph_t* graph, int u, int v) {
    graph->graph[u][v] = 1;
    graph->graph[v][u] = 1;
}

void DFS(graph_t* graph, int v, int parent, int startVertex, int depth) {
    graph->visited[v] = 1;

    int i;
    for (i = 0; i < graph->numVertices; i++) {
        if (graph->graph[v][i]) {
            if (!graph->visited[i]) {
                DFS(graph, i, v, startVertex, depth + 1);
            } else if (i != parent && i == startVertex) {
                if (depth + 1 < graph->minCycleLength) {
                    graph->minCycleLength = depth + 1;
                }
                return;
            }
        }
    }
}

int findShortestCycle(graph_t* graph, int numVertices) {
    int i;
    graph->minCycleLength = INFINITY;

    for (i = 0; i < numVertices; i++) {
        if (!graph->visited[i]) {
            DFS(graph, i, -1, i, 0);
        }
    }

    if (graph->minCycleLength != INFINITY) {
        return graph->minCycleLength;
    } else {
        return -1;  // No cycle found
    }
}

int readInput(graph_t* graph, const char* in_stream) {
    FILE* fp = fopen(in_stream, "r");
    if (!fp)
        return other_error;
    if (fscanf(fp, "%d", &graph->numVertices) != 1) {
        fclose(fp);
        return bad_number_of_lines;
    }
    if (graph->numVertices < 0 || graph->numVertices > MAX_VERTICES) {
        fclose(fp);
        return bad_number_of_vertices;
    }
    if (fscanf(fp, "%d", &graph->numEdges) != 1) {
        fclose(fp);
        return bad_number_of_lines;
    }
    if (graph->numEdges < 0 || graph->numEdges > graph->numVertices * (graph->numVertices - 1) / 2) {
        fclose(fp);
        return bad_number_of_edges;
    }
    initializeGraph(graph, graph->numVertices);

    int from, to;
    for (int i = 0; i < graph->numEdges; ++i) {
        if (fscanf(fp, "%d %d", &from, &to) != 2) {
            fclose(fp);
            return bad_number_of_lines;
        }
        if (from < 0 || to < 0 || from > graph->numVertices || to > graph->numVertices) {
            fclose(fp);
            return bad_vertex;
        }
        addEdge(graph, from, to);
    }
    fclose(fp);
    return no_error;
}

int printError(error_code error, const char* out_stream) {
    FILE* fp = fopen(out_stream, "w");
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

int main(void) {
    //TODO: 1)add destroy_func
    //TODO: 2)make foundCycleLength void
    //TODO: 3)rename graph field to matrix
    //TODO: 4)make 4-5 tescases
    //TODO: 5)add malloc check
    //TODO: 6)fix memory leak
    //TODO: 7)add print res func
    //TODO: 8)replace args to struct fields
    //TODO: 9)add consts
    //TODO: 10)sort includes
    const char *in_stream = "in.txt", *out_stream = "out.txt";
    graph_t graph;
    error_code error = readInput(&graph, in_stream);
    if (error != no_error) {
        printError(error, out_stream);
        return 0;
    }

    int shortestCycleLength = findShortestCycle(&graph, graph.numVertices);

    if (shortestCycleLength == -1) {
        printf("Graph does not contain any cycles.\n");
    } else {
        printf("Length of the shortest cycle: %d\n", shortestCycleLength);
    }

    // Clean up
    for (int i = 0; i < graph.numVertices; i++) {
        free(graph.graph[i]);
    }
    free(graph.graph);
    free(graph.visited);

    return 0;

}