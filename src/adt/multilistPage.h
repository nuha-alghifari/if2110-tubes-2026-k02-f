#ifndef MultiListPage_H
#define MultiListPage_H
#include "linkedlistTag.h"
#include "linkedlistPage.h"
#define Page int //masih temporary

typedef struct nodemlPage* AddressMultiListPage;
typedef struct nodemlPage{
	Page info;
	LinkedlistTag child;
	AddressMultiListPage next;
} NodemlPage;
typedef AddressMultiListPage MultiListPage;


void CreateMultiListPage(MultiListPage *l);
void insertLastMLPage(MultiListPage *l, Page x);
void insertLinkedListPage(MultiListPage *l, Page x, Tag y);
void deleteLinkedListPage(MultiListPage *l, Page x, Tag y);
AddressMultiListPage searchMLPage(MultiListPage l, Page x);
AddressMultiListPage newNodeMLPage(Page x);
void deleteAtMLPage(MultiListPage *l, Page x);
AddressMultiListPage searchMLPageByID(MultiListPage l, int id);

#endif