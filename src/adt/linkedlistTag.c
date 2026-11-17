#include "LinkedlistTag.h"
#include <stdlib.h>

void createListTag(LinkedlistTag *l)
{
    l==NULL;
}
int lengthListTag(LinkedlistTag l)
{
    int length=0;
    AddresslistTag p=l;
    while(p!=NULL)
    {
        length++;
        p=p->next;
    }
    return length;
}
AddresslistTag newNodeTag(ElListTag x)
{
    AddresslistTag new= (AddresslistTag) malloc(sizeof(Node));
    if(new==NULL)
    {
        printf("Alokasi gagal!\n"); //idk if this is allowed
        return NULL;
    }
    else
    {
        new->info=x;
        return new;
    }
}
void insertSortedTag(LinkedlistTag *l, ElListTag x)
{
    AddresslistTag new=newNodeTag(x);
    AddresslistTag p, prec;
    prec=NULL;
    if(*l==NULL)
    {
        *l=new;
    }
    else
    {
        p=*l;
        bool found=false;
        while((p!=NULL) && (!found))
        {
            if(isBeforeTag(x,p->info))
            {
                found=true;
            }
            else
            {
                prec=p;
                p=p->next;
            }
        }
        if(found)
        {
            if(prec==NULL)
            {
                new->next=p;
                *l=new;
            }
            else
            {
                new->next=prec->next;
                prec->next=new;
            }
        }
        else prec->next=new;
    }
}
AddresslistTag searchTag(LinkedlistTag l, ElListTag x)
{
    AddresslistTag p=l;
    bool found=false;
    while((p!=NULL) && (!found))
    {
        if(isTagSama(p->info,x)) found=true;
        else p=p->next;
    }
    if(found) return p;
    else return NULL;
}
void deleteLastTag(LinkedlistTag *l)
{
    AddresslistTag p=*l;
    AddresslistTag prec=NULL;
    while(p->next!=NULL)
    {
        prec=p;
        p=p->next;
    }
    if(prec==NULL)
    {
        *l=NULL;
        free(p);
    }
    else
    {
        prec->next=NULL;
        free(p);
    }
}

void deleteAtTag(LinkedlistTag *l, ElListTag x)
{
    AddresslistTag p=*l;
    AddresslistTag prec=NULL;
    AddresslistTag cari=searchTag(*l,x);
    if(cari!=NULL)
    {
        while(p!=cari)
        {
            prec=p;
            p=p->next;
        }
        if(prec==NULL)
        {
            *l=p->next;
            free(p);
        }
        else
        {
            prec->next=p->next;
            free(p);
        }
    }
}

void deleteAllTag(LinkedlistTag *l)
{
    AddresslistTag p=*l;
    AddresslistTag temp;
    while(p!=NULL)
    {
        temp=p;
        p=p->next;
        free(temp);
    }
    *l=NULL;
}

int lengthTag(LinkedlistTag l)
{
    int lth=0;
    AddresslistTag p=l;
    while(p!=NULL)
    {
        lth++;
        p=p->next;
    }
    return lth;
}