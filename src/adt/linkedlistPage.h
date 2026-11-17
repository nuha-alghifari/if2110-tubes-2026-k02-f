#ifndef LINDKEDLISTPAGE_H
#define LINDKEDLISTPAGE_H
//#include "page.h"
//#include "tag.h"
#define Page int
#define ElListPage Page // Masih temporary
#include <stdbool.h>

//typedef int ElListPage;// Masih temporary

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
bool isBeforePage(Page p1, Page p2);
int lengthPage(LinkedlistPage l);

#endif