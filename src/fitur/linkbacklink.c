#include "linkbacklink.h"

void initLink()
{
    createGraph(&LinkFromPages);
    createGraph(&LinkToPages);
    for(int i=0;i<10;i++)
    {
        insertVertex(&LinkFromPages,1); //Perlu diubah
        insertVertex(&LinkToPages,1);  //Perlu diubah
    }
}

void link()
{
    int idp1, idp2;
    int i1,i2;
    StatusInput s1=TanyaID("Masukkan ID halaman sumber:",'P',idp1);
    if(s1==InputOK)
    {
        i1=SearchVertexIdx(LinkToPages,idp1);
        if(i1!=INVALID_IDX)
        {
            StatusInput s2=TanyaID("Masukkan ID halaman tujuan:",'P',idp2);
            i2=SearchVertexIdx(LinkToPages,idp2);
            if(i2!=INVALID_IDX)
            {
                addEdge(&LinkToPages, idp1, idp2);
                addEdge(&LinkFromPages, idp2, idp1);
                printf("Link berhasil ditambahkan.\n");
            }
            else printf("Invalid page :(\n");
        }
        else printf("Invalid page :(\n");
    }
}

void unlink()
{
    int idp1, idp2;
    int i1,i2;
    StatusInput s1=TanyaID("Masukkan ID halaman sumber:",'P',idp1);
    if(s1==InputOK)
    {
        i1=SearchVertexIdx(LinkToPages,idp1);
        if(i1!=INVALID_IDX)
        {
            StatusInput s2=TanyaID("Masukkan ID halaman tujuan:",'P',idp2);
            i2=SearchVertexIdx(LinkToPages,idp2);
            if(i2!=INVALID_IDX)
            {
                deleteEdge(&LinkToPages, idp1, idp2);
                deleteEdge(&LinkFromPages, idp2, idp1);
                printf("Link berhasil dihapus.\n");
            }
            else printf("Invalid page :(\n");
        }
        else printf("Invalid page :(\n");
    }
}

void links()
{
    int idp;
    int i;
    StatusInput s=TanyaID("Masukkan ID halaman:",'P',idp);
    if(s==InputOK)
    {
        i=SearchVertexIdx(LinkToPages,idp);
        if(i!=INVALID_IDX)
        {
            AddresslistPage p=LinkToPages.node[i].Edge;
            while(p!=NULL)
            {
                printf("[P%d] %s\n",p->info,"test"); //Perlu diganti
                p=p->next;
            }
        }
        else printf("Invalid page :(\n");
    }
}

void backlinks()
{
    int idp;
    int i;
    StatusInput s=TanyaID("Masukkan ID halaman:",'P',idp);
    if(s==InputOK)
    {
        i=SearchVertexIdx(LinkToPages,idp);
        if(i!=INVALID_IDX)
        {
            AddresslistPage p=LinkFromPages.node[i].Edge;
            while(p!=NULL)
            {
                printf("[P%d] %s\n",p->info,"test"); //Perlu diganti
                p=p->next;
            }
        }
        else printf("Invalid page :(\n");
    }
}
