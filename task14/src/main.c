#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int numVertices;
    int numEdges;
    int** adjacencyMatrix;
} Graph;

void initializeGraph(Graph* graph, int numVertices) {
    graph->numVertices = numVertices;
    graph->numEdges = 0;

    graph->adjacencyMatrix = (int**)malloc(numVertices * sizeof(int*));
    int i, j;
    for (i = 0; i < numVertices; i++) {
        graph->adjacencyMatrix[i] = (int*)malloc(numVertices * sizeof(int));
        for (j = 0; j < numVertices; j++) {
            graph->adjacencyMatrix[i][j] = 0;
        }
    }
}

void addEdge(Graph* graph, int source, int destination) {
    graph->adjacencyMatrix[source][destination] = 1;
    graph->adjacencyMatrix[destination][source] = 1;
    graph->numEdges++;
}

int isCommonVertex(Graph* graph, int vertex, int sourceVertex, int destinationVertex) {
    int i;

    for (i = 0; i < graph->numVertices; i++) {
        if (i != vertex && i != sourceVertex && i != destinationVertex) {
            if (graph->adjacencyMatrix[sourceVertex][i] == 1 && graph->adjacencyMatrix[i][destinationVertex] == 1) {
                return 0;
            }
        }
    }

    return 1;
}

int findCommonVertex(Graph* graph, int sourceVertex, int destinationVertex) {
    int i;

    for (i = 0; i < graph->numVertices; i++) {
        if (i != sourceVertex && i != destinationVertex) {
            if (graph->adjacencyMatrix[sourceVertex][i] == 1 && graph->adjacencyMatrix[i][destinationVertex] == 1) {
                if (isCommonVertex(graph, i, sourceVertex, destinationVertex)) {
                    return i;
                }
            }
        }
    }

    return -1;
}

void freeGraph(Graph* graph) {
    int i;
    for (i = 0; i < graph->numVertices; i++) {
        free(graph->adjacencyMatrix[i]);
    }
    free(graph->adjacencyMatrix);
}

int main() {
    int numVertices, numEdges, i;
    int sourceVertex, destinationVertex;
    Graph graph;

    printf("Введите количество вершин в графе: ");
    scanf("%d", &numVertices);

    initializeGraph(&graph, numVertices);

    printf("Введите количество ребер в графе: ");
    scanf("%d", &numEdges);

    printf("Введите ребра в формате 'начальная_вершина конечная_вершина':\n");
    for (i = 0; i < numEdges; i++) {
        int source, destination;
        scanf("%d %d", &source, &destination);
        addEdge(&graph, source, destination);
    }

    printf("Введите исходную вершину: ");
    scanf("%d", &sourceVertex);

    printf("Введите целевую вершину: ");
    scanf("%d", &destinationVertex);

    int commonVertex = findCommonVertex(&graph, sourceVertex, destinationVertex);

    if (commonVertex != -1) {
        printf("Общая вершина, удовлетворяющая условиям, найдена: %d\n", commonVertex);
    } else {
        printf("Общая вершина, удовлетворяющая условиям, не найдена.\n");
    }

    freeGraph(&graph);

    return 0;
}

