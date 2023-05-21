#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 5000

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
    int* commonCands;
    int PathCounter;
    int numVertices;
    int numEdges;
} graph_t;

int initializeGraph(graph_t *graph, const int numVertices) {
    graph->PathCounter = 0;
    graph->commonCands = NULL;
    graph->matrix = (int **)malloc(numVertices * sizeof(int *));
    if(!graph->matrix)
        return other_error;
    for (int i = 0; i < numVertices; i++) {
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
//    graph->matrix[v][u] = 1;
}

void DFS(graph_t* graph, int startVertex, int* visited) {
    visited[startVertex] = 1;

    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->matrix[startVertex][i] && !visited[i]) {
            DFS(graph, i, visited);
        }
    }
}

// Функция для проверки, достижимы ли все вершины из данной вершины
int isReachable(graph_t* graph, int startVertex) {
    int visited[MAX_VERTICES] = {0}; // Массив для отслеживания посещенных вершин

    // Используем алгоритм поиска в глубину (DFS)
    // для отметки всех достижимых вершин
    DFS(graph, startVertex, visited);

    // Проверяем, достижимы ли все вершины из данной вершины
    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            return 0;
        }
    }

    return 1;
}

// Функция для поиска минимального подмножества вершин
void findMinimumVertexSet(graph_t* graph) {
    int minimumVertexSet[MAX_VERTICES] = {0}; // Массив для отслеживания минимального подмножества вершин

    // Проверяем каждую вершину
    for (int i = 0; i < graph->numVertices; i++) {
        if (isReachable(graph, i)) {
            minimumVertexSet[i] = 1;
        }
    }
    // Выводим минимальное подмножество вершин
    printf("Минимальное подмножество вершин: ");
    for (int i = 0; i < graph->numVertices; i++) {
        if (minimumVertexSet[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
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
    findMinimumVertexSet(&graph);
    destroyGraph(&graph);
    return 0;
}