#ifndef LINKBACKLINK_H
#define LINKBACKLINK_H
#include "GraphPage.h"
#include "input.h"

Graph LinkToPages;
Graph LinkFromPages;

void initLink();
void link();
void unlink();
void links();
void backlinks();

#endif