#ifndef TAG_H
#define TAG_H
#define MAXTAGAMOUNT 100 //Masih sembarang
#include <stdbool.h>
#include "kata.h"
#include "jurnal.h"

typedef struct tag
{
	char kata[1024];
	int length;
	int id;
} Tag;

bool isTagValid(Tag t);
Tag addNewTag(int *id);
Tag copyKata(Kata k, int *id);
bool isTagSama(Tag t1, Tag t2);
bool isBeforeTag(Tag t1, Tag t2);

#endif

