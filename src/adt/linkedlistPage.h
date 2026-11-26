#ifndef LINDKEDLISTPAGE_H
#define LINDKEDLISTPAGE_H
//#include "page.h"
//#include "tag.h"
#include <stdbool.h>
#include <stdlib.h>
#include "kata.h"

typedef struct paget{
    Kata title;
    Kata pageid;
} Paget;

typedef Paget ElListPage;

typedef struct nodepage* AddresslistPage;
typedef struct nodepage{
    ElListPage info;
    AddresslistPage next;
} Nodepage;
typedef AddresslistPage LinkedlistPage;

void createListPage(LinkedlistPage *l);
int lengthListPage(LinkedlistPage l);
AddresslistPage newNodePage(ElListPage x);
void insertSortedPage(LinkedlistPage *l, ElListPage x);
AddresslistPage searchPage(LinkedlistPage l, ElListPage x);
void deleteLastPage(LinkedlistPage *l);
void deleteAtPage(LinkedlistPage *l, ElListPage x);
void deleteAllPage(LinkedlistPage *l);
bool isBeforePage(Paget p1, Paget p2);
bool isIDEqualPt(Paget p1, int id);
void geserPage(LinkedlistPage *l);

#endif