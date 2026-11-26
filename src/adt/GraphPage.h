#ifndef GRAPHPAGE_H
#define GRAPHPAGE_H
#define INVALID_IDX -1
#include "linkedlistPage.h"

// Implementasi dengan adjacency list
typedef struct vertex{
    Paget info;
    LinkedlistPage Edge;
} Vertex;

typedef struct graph{
    Vertex node[1000];
    int length_vertex;
} Graph;

void createGraph(Graph *g);
Vertex newVertex(Paget p);
void insertVertex(Graph *g, Paget p);
int SearchVertexIdx(Graph g, Paget p);
int SearchVertexIdxByID(Graph g, int idp);
void addEdge(Graph *g, Paget from, Paget to);
void deleteEdge(Graph *g, Paget from, Paget to);

#endif