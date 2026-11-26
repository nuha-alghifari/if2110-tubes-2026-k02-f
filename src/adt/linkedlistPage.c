#include "linkedlistPage.h"
#include <stdio.h>

void createListPage(LinkedlistPage *l)
{
    *l = NULL;
}
int lengthListPage(LinkedlistPage l)
{
    int length=0;
    AddresslistPage p=l;
    while(p!=NULL)
    {
        length++;
        p=p->next;
    }
    return length;
}
AddresslistPage newNodePage(ElListPage x)
{
    AddresslistPage new= (AddresslistPage) malloc(sizeof(Nodepage));
    if(new==NULL)
    {
        printf("Alokasi gagal!\n"); 
        return NULL;
    }
    else
    {
        new->info=x;
        new->next=NULL;
        return new;
    }
}
void insertSortedPage(LinkedlistPage *l, ElListPage x)
{
    AddresslistPage new=newNodePage(x);
    if(new!=NULL)
    {
        AddresslistPage p, prec;
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
                if(isBeforePage(x,p->info))
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
}
AddresslistPage searchPage(LinkedlistPage l, ElListPage x)
{
    AddresslistPage p=l;
    bool found=false;
    while((p!=NULL) && (!found))
    {
        if(KataSama(&p->info.title,&x.title)) found=true; 
        else p=p->next;
    }
    if(found) return p;
    else return NULL;
}
void deleteLastPage(LinkedlistPage *l)
{
    AddresslistPage p=*l;
    AddresslistPage prec=NULL;
    if(p!=NULL)
    {
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
}

void deleteAtPage(LinkedlistPage *l, ElListPage x)
{
    AddresslistPage p=*l;
    AddresslistPage prec=NULL;
    AddresslistPage cari=searchPage(*l,x);
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
            geserPage(l);
        }
        else
        {
            prec->next=p->next;
            free(p);
            geserPage(&prec->next);
        }
    }
}

void deleteAllPage(LinkedlistPage *l)
{
    AddresslistPage p=*l;
    AddresslistPage temp;
    while(p!=NULL)
    {
        temp=p;
        p=p->next;
        free(temp);
    }
    *l=NULL;
}

bool isBeforePage(Paget p1, Paget p2)
{
    int id1,id2;
    KataKeID(&p1.pageid,'P',&id1);
    KataKeID(&p2.pageid,'P',&id2);
    if(id1<id2) return true;
    else return false;
}

bool isIDEqualPt(Paget p1, int id)
{
    int id1;
    KataKeID(&p1.pageid,'P',&id1);
    if(id1==id) return true;
    else return false;
}

void geserPage(LinkedlistPage *l)
{
    AddresslistPage p=*l;
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