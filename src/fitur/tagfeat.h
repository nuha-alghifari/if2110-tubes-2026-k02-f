#ifndef TAGFEAT_H
#define TAGFEAT_H
#define LENGTHPAGE 10 //Sembarang, perlu diubah nanti
#include "multilistPage.h"
#include "multilistTag.h"
#include "input.h"
#include "jurnal.h"

void tagAdd(Jurnal *j);
void tag(Jurnal *j);
void untag(Jurnal *j);
void tagdelete(Jurnal *j);
void taglist(Jurnal *j);
void pagetags(Jurnal *j);
void tagpages(Jurnal *j);
bool isAlreadyTagged(AddressMultiListPage p, AddressMultiListTag mt);

#endif