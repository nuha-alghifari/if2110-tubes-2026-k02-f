#ifndef MultiListTag_H
#define MultiListTag_H
#include "linkedlistPage.h"
#include "linkedlistTag.h"

typedef struct nodemlTag* AddressMultiListTag;
typedef struct nodemlTag{
	Tag info;
	LinkedlistPage child;
	AddressMultiListTag next;
} NodemlTag;
typedef AddressMultiListTag MultiListTag;


void CreateMultiListTag(MultiListTag *l);
void insertLastMLTag(MultiListTag *l, Tag x);
void insertLinkedListTag(MultiListTag *l, Tag x, Page y);
void deleteLinkedListTag(MultiListTag *l, Tag x, Page y);
AddressMultiListTag searchMLTag(MultiListTag l, Tag x);
AddressMultiListTag newNodeMLTag(Tag x);
void deleteAtMLTag(MultiListTag *l, Tag x);
AddressMultiListTag searchMLTagByID(MultiListTag l, int id);

#endif