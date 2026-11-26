#include <stdio.h>
#include "multilistPage.h"


void CreateMultiListPage(MultiListPage *l)
{
    *l=NULL;
}
void insertLastMLPage(MultiListPage *l, Paget x)
{
    AddressMultiListPage new=newNodeMLPage(x);
    if(new!=NULL)
    {
        if(*l==NULL)
        {
            *l=new;
        }
        else
        {
            AddressMultiListPage p=*l;
            while(p->next!=NULL)
            {
                p=p->next;
            }
            p->next=new;
        }
    }
}
void insertLinkedListPage(MultiListPage *l, Paget x, Tag y)
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
void deleteLinkedListPage(MultiListPage *l, Paget x, Tag y)
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

AddressMultiListPage searchMLPage(MultiListPage l, Paget x)
{
    AddressMultiListPage p=l;
    bool found=false;
    while((p!=NULL) && (!found))
    {
        if(KataSama(&p->info.title,&x.title)) found=true; //Perlu diubah
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
        if(isIDEqualPt(p->info,id)) found=true; //Perlu diubah
        else p=p->next;
    }
    if(found) return p;
    else return NULL;
}

AddressMultiListPage newNodeMLPage(Paget x)
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
        new->next=NULL;
        createListTag(&new->child);
        return new;
    }
}

void deleteAtMLPage(MultiListPage *l, Paget x)
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
            deleteAllTag(&p->child);
            free(p);
            geserPageML(l);
        }
        else
        {
            prec->next=p->next;
            deleteAllTag(&p->child);
            free(p);
            geserPageML(&prec->next);
        }
    }
}

void geserPageML(MultiListPage *l)
{
    AddressMultiListPage p=*l;
    int id;
    char buf[16];
    while(p!=NULL)
    {
        KataKeID(&p->info.pageid,'P',&id);
        id--;
        sprintf(buf, "P%d", id);
        KataDariC(&p->info.pageid,buf);
        p=p->next;
    }
}