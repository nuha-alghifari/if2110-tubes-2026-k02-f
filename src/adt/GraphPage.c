#include "GraphPage.h"

void createGraph(Graph *g)
{
    g->length_vertex=0;
}

Vertex newVertex(Page p)
{
    Vertex v;
    v.info=p;
    createListPage(&v.Edge);
}

void insertVertex(Graph *g, Page p)
{
    if(g->length_vertex<1000) g->node[g->length_vertex]=newVertex(p);
    else
    {
        for(int i=0;i<999;i++)
        {
            g->node[i]=g->node[i+1];
        }
        g->node[999]=newVertex(p);
    }
}

int SearchVertexIdx(Graph g, Page p)
{
    int id=INVALID_IDX;
    for(int i=0;i<g.length_vertex;i++)
    {
        if(g.node[i].info==p) 
        {
            id=i;
            break;
        }
    }
    return id;
}

void addEdge(Graph *g, Page from, Page to)
{
    int id1=SearchVertex(*g,from);
    int id2=SearchVertex(*g,to);
    if((id1!=INVALID_IDX) && (id2!=INVALID_IDX))
    {
        insertSortedPage(&g->node[id1].Edge,to);
    }
}

void deleteEdge(Graph *g, Page from, Page to)
{
    int id1=SearchVertex(*g,from);
    int id2=SearchVertex(*g,to);
    if((id1!=INVALID_IDX) && (id2!=INVALID_IDX))
    {
        deleteAtPage(&g->node[id1].Edge,to);
    }
}