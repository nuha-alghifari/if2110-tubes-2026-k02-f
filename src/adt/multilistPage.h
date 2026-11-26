#ifndef MultiListPage_H
#define MultiListPage_H
#include "linkedlistTag.h"
#include "linkedlistPage.h"

typedef struct nodemlPage* AddressMultiListPage;
typedef struct nodemlPage{
	Paget info;
	LinkedlistTag child;
	AddressMultiListPage next;
} NodemlPage;
typedef AddressMultiListPage MultiListPage;


void CreateMultiListPage(MultiListPage *l);
void insertLastMLPage(MultiListPage *l, Paget x);
void insertLinkedListPage(MultiListPage *l, Paget x, Tag y);
void deleteLinkedListPage(MultiListPage *l, Paget x, Tag y);
AddressMultiListPage searchMLPage(MultiListPage l, Paget x);
AddressMultiListPage newNodeMLPage(Paget x);
void deleteAtMLPage(MultiListPage *l, Paget x);
AddressMultiListPage searchMLPageByID(MultiListPage l, int id);
void geserPageML(MultiListPage *l);

#endif