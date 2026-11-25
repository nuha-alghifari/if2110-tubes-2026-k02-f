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
        printf("Alokasi gagal!\n"); //idk if this is allowed
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
                if(isBeforePage(x,p->info)) //Masih temp, perlu diganti
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
        if(p->info==x) found=true; //Perlu diubah
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
        }
        else
        {
            prec->next=p->next;
            free(p);
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

bool isBeforePage(Page p1, Page p2) //Perlu diganti
{
    if(p1<p2) return true;
    else return false;
}

