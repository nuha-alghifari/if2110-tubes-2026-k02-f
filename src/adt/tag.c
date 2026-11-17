#include "stdio.h"
#include "Tag.h"
#include "mesin_karakter.h"
#include <stdlib.h>
bool isTagValid(Tag t)
{
	bool valid;
    int i=0;
	while ((i<t.length) && (valid==true))
	{
		valid=false;
		for(char c = 'a'; c <= 'z' ; c++)
		{
			if (t.kata[i] == c)
				valid = true;
		}
		for(char c= '1'; c <= '9' ; c++)
		{
			if (t.kata[i] == c)
				valid = true;
		}
		if((t.kata[i]=='-') || (t.kata[i]=='_')) valid=true;
        i++;
	}
	return valid;
}
Tag createNewTag()
{
	Tag t;
	t.length=0;
	t.id=idcounter;
	idcounter++;
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
}

bool isTagSama(Tag t1, Tag t2)
{
    if(t1.length==t2.length)
    {
        bool sama=true;
        for(int i=0;i<t1.length;i++)
        {
            if(t1.kata[i]!=t2.kata)
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
	while((i<t1.length) && (i<t2.length))
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

