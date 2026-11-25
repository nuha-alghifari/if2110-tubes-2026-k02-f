#include "tagfeat.h"

void initTag()
{
	CreateMultiListPage(&pagetag);
	for (int i = 0; i < LENGTHPAGE; i++)
		insertLastMLPage(&pagetag, 1);
	CreateMultiListTag(&tagpage);
}

void tagAdd()
{
    Kata tagname;
    StatusInput s=Tanya("Masukkan nama tag:",&tagname);
    if(s==InputOK)
    {
        Tag t = copyKata(tagname);
        if(isTagValid(t))
        {
            AddressMultiListTag p = searchMLTag(tagpage, t);
            if (p!=NULL)
                printf("Tag sudah ada :(");
            else
            {
                insertLastMLTag(&tagpage, t);
                printf("Tag T%d berhasil dibuat",idcounter-1);
            }
        }
        else printf("Nama tag tidak valid!");
    }
}

void tag()
{
    AddressMultiListPage p;
    AddressMultiListTag t;
    int idp,idt;
    StatusInput s1=TanyaID("Masukkan ID halaman:",'P',&idp);
    if((s1==InputOK))
    {
        p=searchMLPageByID(pagetag,idp);
        if(p!=NULL)
        {
            StatusInput s2=TanyaID("Masukkan ID tag:",'T',&idt);
            if(s2==InputOK)
            {
                t=searchMLTagByID(tagpage,idt);
                if(t!=NULL)
                {
                    if(isAlreadyTagged(p,t))
                    {
                        printf("Already tagged :(");
                    }
                    else
                    {
                        insertLinkedListTag(&tagpage, t->info, p->info);
                        insertLinkedListPage(&pagetag, p->info, t->info);
                        printf("Tag berhasil dipasang.");
                    }
                }
                else printf("Invalid tag :(");
            }
        }
        else printf("Invalid page :(");
    }
}

void untag()
{
	AddresslistPage p;
    AddresslistTag t;
    int idp,idt;
    StatusInput s1=TanyaID("Masukkan ID halaman:",'P',&idp);
    if((s1==InputOK))
    {
        p=searchMLPageByID(pagetag,idp);
        if(p!=NULL)
        {
            StatusInput s2=TanyaID("Masukkan ID tag:",'T',&idt);
            if(s2==InputOK)
            {
                t=searchMLTagByID(tagpage,idt);
                if(t!=NULL)
                {
                    deleteLinkedListTag(&tagpage, t->info, p->info);
	                deleteLinkedListPage(&pagetag, p->info, t->info);
                    printf("Tag berhasil dilepas.");
                }
                else printf("Invalid tag :(");
            }
        }
        else printf("Invalid page :(");
    }
}

void tagdelete()
{
    int idt;
	StatusInput s1=TanyaID("Masukkan ID tag:",'T',&idt);
    if(s1==InputOK)
    {
        AddressMultiListTag t=searchMLTagByID(tagpage,idt);
        if(t!=NULL)
        {
            bool ya;
            printf("Hapus tag %s dari katalog dan seluruh halaman? (Y/N)",t->info);
            StatusInput s2=TanyaYN(NULL,&ya);
            if(ya)
            {
                AddresslistPage tp=t->child;
                while(tp!=NULL)
                {
                    deleteLinkedListPage(&pagetag, tp->info, t->info);
                    tp=tp->next;
                }
                deleteAtMLTag(&tagpage, t->info);
                printf("Tag berhasil dihapus.");
            }
        }
        else printf("Invalid tag :(");
    }
}

void taglist()
{
    AddressMultiListTag p= tagpage;
    while(p!=NULL)
    {
        printf("[T%d] %s - %d halaman\n", p->info.id, p->info.kata, lengthListTag(p->child));
        p=p->next;
    }
}

void pagetags()
{
    int idp;
	StatusInput s1=TanyaID("Masukkan ID page:",'P',&idp);
    if(s1==InputOK)
    {
        AddressMultiListPage p=searchMLPageByID(pagetag,idp);
        if(p!=NULL)
        {
            AddresslistTag t=p->child;
            while(t!=NULL)
            {
                printf("[P%d] %s", t->info.id, t->info.kata);
                t=t->next;
            }
            return;
        }
        else printf("Invalid page :(");
    }
}

void tagpages()
{
    int idt;
	StatusInput s1=TanyaID("Masukkan ID tag:",'T',&idt);
    if(s1==InputOK)
    {
        AddressMultiListPage t=searchMLTagByID(pagetag,idt);
        if(t!=NULL)
        {
            AddresslistPage p=t->child;
            while(p!=NULL)
            {
                printf("[T%d] %s", p->info, "test"); //Belum diimplementasi
                p=p->next;
            }
            return;
        }
        else printf("Invalid tag :(");
    }
}

bool isAlreadyTagged(AddressMultiListPage p, AddressMultiListTag mt)
{
    AddresslistTag t=searchTag(p->child,mt->info);
    if(t!=NULL) return false;
    else return true;
}

// bool isTagIdxValid(int id)
// {
//     AddressMultiListTag ada= searchMLTagByID(id);
//     return ada!=NULL;
// }
// int idcounter

// tag
// int id

// createnewtag

// id=idcounter
// idcounter++


// bool isTaggingValid()
// {
// 	isPageValid();
// 	isTagValid();
// }


	
// bool isTagValid(char *t)
// {
//     int angka[10];
// 	if(t[0]=='T')
// 	{
// 		if(isAngka(t))
// 		{
//             for (int i = 1 ; i < length(t); i++)
//             {
//                 angka[i - 1] = t[i];
//             }
//             angka[length(t)] = '\0';
// 		}
// 		int id=atol(angka);
// 		if(id>0 && id<idcounter) return true;
// 		else return false;
// 	}
// }

// bool isAngka(char *t)
// {
// 	int i=1;
//     bool valid=true;
// 	while ((i<length(t)) && (valid==true))
// 	{
// 		valid=false;
// 		for(char c='0';c<='9';c++)
//         {
// 			if(t[i]==c) valid=true;
//         }
// 	}
// 	return valid;
// }
	
// int length(char *c)
// {
// 	int i=0;
// 	while(c[i]!='\0') i++;
// 	return i;
// }
