#include "linkbacklink.h"
#include <stdio.h>


void link(Jurnal *j)
{
    int idp1, idp2;
    int i1,i2;
    StatusInput s1=TanyaID("Masukkan ID halaman sumber:",'P', &idp1);
    if(s1==InputOK)
    {
        i1=SearchVertexIdxByID(j->linkKeluar,idp1);
        if(i1!=INVALID_IDX)
        {
            StatusInput s2=TanyaID("Masukkan ID halaman tujuan:",'P',idp2);
            i2=SearchVertexIdxByID(j->linkKeluar,idp2);
            if(i2!=INVALID_IDX)
            {
                addEdge(&j->linkKeluar, j->linkKeluar.node[i1].info, j->linkKeluar.node[i2].info);
                addEdge(&j->linkMasuk, j->linkKeluar.node[i2].info, j->linkKeluar.node[i1].info);
                printf("Link berhasil ditambahkan.\n");
            }
            else printf("Invalid page :(\n");
        }
        else printf("Invalid page :(\n");
    }
}

void unlink(Jurnal *j)
{
    int idp1, idp2;
    int i1,i2;
    StatusInput s1=TanyaID("Masukkan ID halaman sumber:",'P',idp1);
    if(s1==InputOK)
    {
        i1=SearchVertexIdxByID(j->linkKeluar,idp1);
        if(i1!=INVALID_IDX)
        {
            StatusInput s2=TanyaID("Masukkan ID halaman tujuan:",'P',idp2);
            i2=SearchVertexIdxByID(j->linkKeluar,idp2);
            if(i2!=INVALID_IDX)
            {
                deleteEdge(&j->linkKeluar, j->linkKeluar.node[i1].info, j->linkKeluar.node[i2].info);
                deleteEdge(&j->linkMasuk, j->linkKeluar.node[i2].info, j->linkKeluar.node[i1].info);
                printf("Link berhasil dihapus.\n");
            }
            else printf("Invalid page :(\n");
        }
        else printf("Invalid page :(\n");
    }
}

void links(Jurnal *j)
{
    int idp;
    int i;
    StatusInput s=TanyaID("Masukkan ID halaman:",'P',idp);
    if(s==InputOK)
    {
        i=SearchVertexIdxByID(j->linkKeluar,idp);
        if(i!=INVALID_IDX)
        {
            AddresslistPage p=j->linkKeluar.node[i].Edge;
            while(p!=NULL)
            {
                printf("[P%d] %s\n",p->info,"test"); //Perlu diganti
                p=p->next;
            }
        }
        else printf("Invalid page :(\n");
    }
}

void backlinks(Jurnal *j)
{
    int idp;
    int i;
    StatusInput s=TanyaID("Masukkan ID halaman:",'P',idp);
    if(s==InputOK)
    {
        i=SearchVertexIdxByID(j->linkKeluar,idp);
        if(i!=INVALID_IDX)
        {
            AddresslistPage p=j->linkMasuk.node[i].Edge;
            while(p!=NULL)
            {
                printf("[P%d] %s\n",p->info,"test"); //Perlu diganti
                p=p->next;
            }
        }
        else printf("Invalid page :(\n");
    }
}
