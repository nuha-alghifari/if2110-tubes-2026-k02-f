#include <stdio.h>
#include "MultiListPage.h"


void CreateMultiListPage(MultiListPage *l)
{
    l=NULL;
}
void insertLastMLPage(MultiListPage *l, Page x)
{
    AddressMultiListPage new=newNodeMLPage(x);
    if(*l==NULL)
    {
        *l=new;
    }
    else
    {
        AddressMultiListPage p=*l;
        //AddressMultiListPage prec=NULL;
        while(p->next!=NULL)
        {
            p=p->next;
        }
        p->next=new;
    }
}
void insertLinkedListPage(MultiListPage *l, Page x, Tag y)
{
    AddressMultiListPage cari=searchMLPage(*l,x);
    if(cari!=NULL)
    {
        AddressMultiListPage p=*l;
        while(p!=cari)
        {
            p=p->next;
        }
        insertSortedTag(&p->child,y);
    }
}
void deleteLinkedListPage(MultiListPage *l, Page x, Tag y)
{
    AddressMultiListPage cari=searchMLPage(*l,x);
    if(cari!=NULL)
    {
        AddressMultiListPage p=*l;
        while(p!=cari)
        {
            p=p->next;
        }
        deleteAtTag(&p->child,y);
    }
}

AddressMultiListPage searchMLPage(MultiListPage l, Page x)
{
    AddressMultiListPage p=l;
    bool found=false;
    while((p!=NULL) && (!found))
    {
        if(p->info==x) found=true; //Perlu diubah
        else p=p->next;
    }
    if(found) return p;
    else return NULL;
}

AddressMultiListPage searchMLPageByID(MultiListPage l, int id)
{
    AddressMultiListPage p=l;
    bool found=false;
    while((p!=NULL) && (!found))
    {
        if(p->info==id) found=true; //Perlu diubah
        else p=p->next;
    }
    if(found) return p;
    else return NULL;
}

AddressMultiListPage newNodeMLPage(Page x)
{
    AddressMultiListPage new= (AddressMultiListPage) malloc(sizeof(NodemlPage));
    if(new==NULL)
    {
        printf("Alokasi gagal!\n");
        return NULL;
    }
    else
    {
        new->info=x;
        return new;
    }
}

void deleteAtMLPage(MultiListPage *l, Page x)
{
    AddressMultiListPage p= *l;
    AddressMultiListPage cari= searchMLPage(*l,x);
    if(cari!=NULL)
    {
        AddressMultiListPage prec=NULL;
        while(p!=cari)
        {
            prec=p;
            p=p->next;
        }
        if(prec==NULL)
        {
            *l=p->next;
            free(p); //Apakah ini cukup?
        }
        else
        {
            prec->next=p->next;
            free(p);
        }
    }
}