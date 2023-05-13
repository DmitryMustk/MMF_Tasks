#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

typedef struct graph_t {
    int ver_num;
    int edge_num;
    int** graph;
} graph_t;

// Функция для создания графа с заданным количеством вершин
graph_t* createGraph(int ver_num) {
    graph_t* graph = malloc(sizeof(graph_t));
    graph->ver_num = ver_num;
    graph->edge_num = 0;

    // Выделяем память для матрицы смежности
    graph->graph = malloc(ver_num * sizeof(int*));
    for (int i = 0; i < ver_num; i++) {
        graph->graph[i] = malloc(ver_num * sizeof(int));
        for (int j = 0; j < ver_num; j++) {
            graph->graph[i][j] = INF;
        }
    }

    return graph;
}

// Функция для установки значения графа по индексам вершин
void setGraphValue(graph_t* graph, int row, int col, int value) {
    graph->graph[row][col] = value;
    graph->edge_num++;
}

// Функция для нахождения длины кратчайшего цикла в графе
int shortestCycle(graph_t* graph) {
    int dist[graph->ver_num][graph->ver_num];

    // Инициализируем расстояния между всеми парами вершин
    for (int i = 0; i < graph->ver_num; i++) {
        for (int j = 0; j < graph->ver_num; j++) {
            dist[i][j] = graph->graph[i][j];
        }
    }

    // Применяем алгоритм Флойда-Уоршелла для нахождения кратчайших расстояний
    for (int k = 0; k < graph->ver_num; k++) {
        for (int i = 0; i < graph->ver_num; i++) {
            for (int j = 0; j < graph->ver_num; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    int minCycle = INF;

    // Проверяем все циклы, начинающиеся и заканчивающиеся в одной и той же вершине
    for (int i = 0; i < graph->ver_num; i++) {
        if (dist[i][i] < minCycle) {
            minCycle = dist[i][i];
        }
    }

    return (minCycle == INF) ? -1 : minCycle; // Если цикл не найден, возвращаем -1
}

int main() {
    graph_t* graph = createGraph(30);

    // Заполняем граф значениями
    setGraphValue(graph, 0, 1, 1);
    setGraphValue(graph, 0, 2, 1);
    setGraphValue(graph, 0, 3, 1);
    setGraphValue(graph, 1, 4, 1);
    setGraphValue(graph, 1, 5, 1);
    setGraphValue(graph, 2, 4, 1);
    setGraphValue(graph, 2, 6, 1);
    setGraphValue(graph, 3, 5, 1);
    setGraphValue(graph, 3, 6, 1);
    setGraphValue(graph, 4, 7, 1);
    setGraphValue(graph, 5, 8, 1);
    setGraphValue(graph, 6, 9, 1);
    setGraphValue(graph, 7, 10, 1);
    setGraphValue(graph, 8, 10, 1);
    setGraphValue(graph, 9, 11, 1);
    setGraphValue(graph, 11, 10, 1);
    setGraphValue(graph, 10, 12, 1);
    setGraphValue(graph, 12, 13, 1);
    setGraphValue(graph, 12, 14, 1);
    setGraphValue(graph, 14, 15, 1);
    setGraphValue(graph, 14, 16, 1);
    setGraphValue(graph, 16, 17, 1);
    setGraphValue(graph, 16, 18, 1);
    setGraphValue(graph, 18, 19, 1);
    setGraphValue(graph, 18, 20, 1);
    setGraphValue(graph, 20, 21, 1);
    setGraphValue(graph, 20, 22, 1);
    setGraphValue(graph, 22, 23, 1);
    setGraphValue(graph, 22, 24, 1);
    setGraphValue(graph, 24, 25, 1);
    setGraphValue(graph, 24, 26, 1);
    setGraphValue(graph, 26, 27, 1);
    setGraphValue(graph, 26, 28, 1);
    setGraphValue(graph, 28, 29, 1);

    // Добавляем дополнительные ребра для образования цикла с обхватом 8
    setGraphValue(graph, 7, 15, 1);
    setGraphValue(graph, 15, 23, 1);
    setGraphValue(graph, 23, 29, 1);
    setGraphValue(graph, 29, 11, 1);
    setGraphValue(graph, 11, 19, 1);
    setGraphValue(graph, 19, 27, 1);
    setGraphValue(graph, 27, 17, 1);
    setGraphValue(graph, 17, 9, 1);
    int shortestCycleLength = shortestCycle(graph);
    printf("%d\n", shortestCycleLength);
    return 0;
}