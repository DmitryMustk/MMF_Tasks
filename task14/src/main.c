#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 5000

int path[MAX_VERTICES];
int pathIndex = 0;

typedef enum error_code {
    no_error = 0,
    other_error,
    bad_number_of_lines,
    bad_number_of_vertices,
    bad_number_of_edges,
    bad_vertex,
} error_code;

typedef struct {
    int **matrix;
    int *visited;
    int* commonCands;
    int PathCounter;
    int numVertices;
    int numEdges;
} graph_t;

int initializeGraph(graph_t *graph, const int numVertices) {
    graph->PathCounter = 0;
    graph->visited = (int *)malloc(numVertices * sizeof(int));
    if(!graph->visited)
        return other_error;
    graph->commonCands = NULL;
    graph->matrix = (int **)malloc(numVertices * sizeof(int *));
    if(!graph->matrix)
        return other_error;
    for (int i = 0; i < numVertices; i++) {
        graph->visited[i] = 0;
        graph->matrix[i] = (int *) malloc(numVertices * sizeof(int));
        if(!graph->matrix[i])
            return other_error;
        for (int j = 0; j < numVertices; j++) {
            graph->matrix[i][j] = 0;
        }
    }
    return no_error;
}

void addEdge(graph_t *graph, const int u, const int v) {
    graph->matrix[u][v] = 1;
    graph->matrix[v][u] = 1;
}
void printPath(int path[], int length) {
    int i;
    for (i = 0; i < length; i++) {
        printf("%d ", path[i]);
    }
    printf("\n");
}

void findCommonCands(graph_t* graph, int path[], int length){
    if(graph->commonCands == NULL){
        graph->commonCands = calloc(graph->numVertices, sizeof(int));
        if(!graph->commonCands)
            exit(0);
        for(int i = 1; i < length - 1; ++i){
            graph->commonCands[i] = path[i];
        }
    }
    int flag = 0;
    for(int i = 0; i < graph->numVertices; ++i){
        for(int j = 1; j < length - 1; ++j){
            if(graph->commonCands[i] != 0 && graph->commonCands[i] == path[j]) {
                flag = 1;
                break;
            }
        }
        if(graph->commonCands[i] != 0 && !flag)
            graph->commonCands[i] = 0;
    }
}

void DFS(graph_t* graph, int start, int end) {
    graph->visited[start] = 1;
    path[pathIndex++] = start;

    if (start == end) {
        printPath(path, pathIndex);
        findCommonCands(graph, path, pathIndex);
    } else {
        int i;
        for (i = 0; i < graph->numVertices; i++) {
            if (graph->matrix[start][i] == 1 && !graph->visited[i]) {
                DFS(graph, i, end);
            }
        }
    }

    graph->visited[start] = 0;
    pathIndex--;
}

int readInput(graph_t *graph, const char *in_stream) {
    FILE *fp = fopen(in_stream, "r");
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

int printError(const error_code error, const char *out_stream) {
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

void destroyGraph(graph_t* graph){
    for (int i = 0; i < graph->numVertices; i++) {
        free(graph->matrix[i]);
    }
    free(graph->commonCands);
    free(graph->matrix);
    free(graph->visited);
}

int main(void) {
    const char *in_stream = "in.txt", *out_stream = "out.txt";
    graph_t graph;
    error_code error = readInput(&graph, in_stream);
    if (error != no_error) {
        printError(error, out_stream);
        destroyGraph(&graph);
        return 0;
    }
    int from, to;
    printf("Введите две вершины, между которыми нужно найти общую:\n");
    scanf("%d %d", &from, &to);

    DFS(&graph, from, to);
    for(int i = 0; i < graph.numVertices; ++i){
        if(graph.commonCands[i] != 0)
            printf("%d ", graph.commonCands[i]);
    }
    destroyGraph(&graph);
    return 0;
}
