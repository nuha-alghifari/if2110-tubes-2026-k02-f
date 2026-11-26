#include "stdio.h"
#include "tag.h"
#include "mesin_karakter.h"
#include <stdlib.h>

bool isTagValid(Tag t)
{
	int i;
	if (t.length <= 0) return false;
	for (i = 0; i < t.length; i++)
	{
		char c = t.kata[i];
		bool ok = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_';
		if (!ok) return false;
	}
	return true;
}

Tag createNewTag()
{
	Tag t;
	t.length=0;
	t.id=0; 
	return t;
}

Tag copyKata(Kata k)
{
	Tag t=createNewTag();
	for(int i=0;i<k.length;i++)
	{
		t.kata[i]=k.tab[i];
	}
	t.kata[k.length]='\0';
	t.length=k.length;
	return t;
}

bool isTagSama(Tag t1, Tag t2)
{
    if(t1.length==t2.length)
    {
        bool sama=true;
        for(int i=0;i<t1.length;i++)
        {
            if(t1.kata[i]!=t2.kata[i])
            {
                sama=false;
                break;
            }
        }
        return sama;
    }
    else return false;
}

bool isBeforeTag(Tag t1, Tag t2)
{
	int i=0;
	bool before;
	bool selesai=false;
	while((i<t1.length) && (i<t2.length) &&(!selesai))
	{
		if(t1.kata[i]==t2.kata[i])
		{
			i++;
		}
		else if(t1.kata[i]<t2.kata[i])
		{
			selesai=true;
			before=true;
		}
		else
		{
			selesai=true;
			before=false;
		}
	}
	if(selesai) return before;
	else
	{
		if(t1.length<t2.length) return true;
		else return false;
	}
}