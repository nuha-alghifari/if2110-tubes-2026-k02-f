#ifndef GRAPHPAGE_H
#define GRAPHPAGE_H
#define INVALID_IDX -1
#include "linkedlistPage.h"

// Implementasi dengan adjacency list
typedef struct vertex{
    Page info;
    LinkedlistPage Edge;
} Vertex;

typedef struct graph{
    Vertex node[1000];
    int length_vertex;
} Graph;

void createGraph(Graph *g);
Vertex newVertex(Page p);
void insertVertex(Graph *g, Page p);
int SearchVertexIdx(Graph g, Page p);
void addEdge(Graph *g, Page from, Page to);
void deleteEdge(Graph *g, Page from, Page to);

#endif