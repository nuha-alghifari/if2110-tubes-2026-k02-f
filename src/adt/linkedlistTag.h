#ifndef LINKEDLISTTAG_H
#define LINKEDLISTTAG_H
#include "tag.h"
#include <stdlib.h>
#define ElListTag Tag

typedef struct node* AddresslistTag;
typedef struct node{
    ElListTag info;
    AddresslistTag next;
} Node;
typedef AddresslistTag LinkedlistTag;

void createListTag(LinkedlistTag *l);
int lengthListTag(LinkedlistTag l);
AddresslistTag newNodeTag(ElListTag x);
void insertSortedTag(LinkedlistTag *l, ElListTag x);
AddresslistTag searchTag(LinkedlistTag l, ElListTag x);
void deleteLastTag(LinkedlistTag *l);
void deleteAtTag(LinkedlistTag *l, ElListTag x);
void deleteAllTag(LinkedlistTag *l);

#endif