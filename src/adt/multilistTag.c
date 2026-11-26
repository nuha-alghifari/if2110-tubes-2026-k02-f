#include <stdio.h>
#include "multilistTag.h"


void CreateMultiListTag(MultiListTag *l)
{
    *l=NULL;
}
void insertLastMLTag(MultiListTag *l, Tag x)
{
    AddressMultiListTag new=newNodeMLTag(x);
    if(new!=NULL)
    {
        if(*l==NULL)
        {
            *l=new;
        }
        else
        {
            AddressMultiListTag p=*l;
            //AddressMultiListTag prec=NULL;
            while(p->next!=NULL)
            {
                p=p->next;
            }
            p->next=new;
        }
    }
}
void insertLinkedListTag(MultiListTag *l, Tag x, Paget y)
{
    AddressMultiListTag cari=searchMLTag(*l,x);
    if(cari!=NULL)
    {
        AddressMultiListTag p=*l;
        while(p!=cari)
        {
            p=p->next;
        }
        insertSortedPage(&p->child,y);
    }
}
void deleteLinkedListTag(MultiListTag *l, Tag x, Paget y)
{
    AddressMultiListTag cari=searchMLTag(*l,x);
    if(cari!=NULL)
    {
        AddressMultiListTag p=*l;
        while(p!=cari)
        {
            p=p->next;
        }
        deleteAtPage(&p->child,y);
    }
}

AddressMultiListTag searchMLTag(MultiListTag l, Tag x)
{
    AddressMultiListTag p=l;
    bool found=false;
    while((p!=NULL) && (!found))
    {
        if(isTagSama(p->info,x)) found=true;
        else p=p->next;
    }
    if(found) return p;
    else return NULL;
}

AddressMultiListTag searchMLTagByID(MultiListTag l, int id)
{
    AddressMultiListTag p=l;
    bool found=false;
    while((p!=NULL) && (!found))
    {
        if(p->info.id==id) found=true;
        else p=p->next;
    }
    if(found) return p;
    else return NULL;
}

AddressMultiListTag newNodeMLTag(Tag x)
{
    AddressMultiListTag new= (AddressMultiListTag) malloc(sizeof(NodemlTag));
    if(new==NULL)
    {
        printf("Alokasi gagal!\n");
        return NULL;
    }
    else
    {
        new->info=x;
        new->next=NULL;
        createListPage(&new->child);
        return new;
    }
}

void deleteAtMLTag(MultiListTag *l, Tag x)
{
    AddressMultiListTag p= *l;
    AddressMultiListTag cari= searchMLTag(*l,x);
    if(cari!=NULL)
    {
        AddressMultiListTag prec=NULL;
        while(p!=cari)
        {
            prec=p;
            p=p->next;
        }
        if(prec==NULL)
        {
            *l=p->next;
            deleteAllPage(&p->child);
            free(p); 
            geserTagML(l);
        }
        else
        {
            prec->next=p->next;
            deleteAllPage(&p->child);
            free(p);
            geserTagML(&prec->next);
        }
    }
}

void geserTagML(MultiListTag *l)
{
    AddressMultiListTag p=*l;
    while(p!=NULL)
    {
        p->info.id--;
        p=p->next;
    }
}