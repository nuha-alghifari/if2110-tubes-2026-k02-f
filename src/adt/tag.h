#ifndef TAG_H
#define TAG_H
#define MAXTAGAMOUNT 100 
#include <stdbool.h>
#include "kata.h"

typedef struct tag
{
	char kata[1024];
	int length;
	int id;
} Tag;

bool isTagValid(Tag t);
Tag copyKata(Kata k);
bool isTagSama(Tag t1, Tag t2);
bool isBeforeTag(Tag t1, Tag t2);

#endif