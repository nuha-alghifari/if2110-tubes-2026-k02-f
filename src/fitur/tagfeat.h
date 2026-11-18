#ifndef TAGFEAT_H
#define TAGFEAT_H
#define LENGTHPAGE 10 //Sembarang, perlu diubah nanti
#include "multilistPage.h"
#include "multilistTag.h"
#include <stdlib.h>
#include "input.h"

MultiListPage pagetag;
MultiListTag tagpage;

void initTag();
void tagAdd();
void tag();
void untag();
void tagdelete();
void taglist();
void pagetags();
void tagpages();
bool isAlreadyTagged(AddressMultiListPage p, AddressMultiListTag mt);

#endif