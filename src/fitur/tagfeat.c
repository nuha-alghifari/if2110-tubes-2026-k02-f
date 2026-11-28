#include <stdio.h>
#include "tagfeat.h"
#include "input.h"
#include "tag.h"

static bool TanyaHalamanTag(Jurnal *J, AddressMultiListPage *out, int *idp) {
    if (TanyaID("Masukkan ID halaman:", 'P', idp) != InputOK) return false;
    *out = searchMLPageByID(J->pageTags, *idp);
    if (*out == NULL) {
        printf("Halaman P%d tidak ditemukan.\n", *idp);
        return false;
    }
    return true;
}

static bool TanyaTagKatalog(Jurnal *J, AddressMultiListTag *out, int *idt) {
    if (TanyaID("Masukkan ID tag:", 'T', idt) != InputOK) return false;
    *out = searchMLTagByID(J->tagPages, *idt);
    if (*out == NULL) {
        printf("Tag T%d tidak ditemukan.\n", *idt);
        return false;
    }
    return true;
}

void TagTambahHalaman(Jurnal *J, int idHalaman) {
    if (searchMLPageByID(J->pageTags, idHalaman) == NULL)
        insertLastMLPage(&J->pageTags, idHalaman);
}

void TagHapusHalaman(Jurnal *J, int idHalaman) {
    AddressMultiListPage np = searchMLPageByID(J->pageTags, idHalaman);
    AddresslistTag t;
    if (np == NULL) return;
    for (t = np->child; t != NULL; t = t->next)
        deleteLinkedListTag(&J->tagPages, t->info, idHalaman);
    deleteAtMLPage(&J->pageTags, idHalaman);
}

void CMDTagAdd(Jurnal *J) {
    Kata nama;
    Tag t;
    if (TanyaTidakKosong("Masukkan nama tag:", &nama) != InputOK) return;
    t = copyKata(nama);
    if (!isTagValid(t)) {
        printf("Nama tag tidak valid. Gunakan huruf kecil, angka, _ atau -.\n");
        return;
    }
    if (searchMLTag(J->tagPages, t) != NULL) {
        printf("Tag \"%s\" sudah ada.\n", t.kata);
        return;
    }
    t.id = AmbilIDTag(J);   // ID baru hanya dipakai kalau tag lolos validasi
    insertLastMLTag(&J->tagPages, t);
    TrieTambah(&J->trieTag, t.kata, t.id);
    TandaiUbah(J);
    printf("Tag T%d berhasil dibuat.\n", t.id);
}

void CMDTag(Jurnal *J) {
    AddressMultiListPage np;
    AddressMultiListTag nt;
    int idp, idt;
    if (!TanyaHalamanTag(J, &np, &idp)) return;
    if (!TanyaTagKatalog(J, &nt, &idt)) return;
    if (searchTag(np->child, nt->info) != NULL) {
        printf("Halaman P%d sudah memiliki tag T%d.\n", idp, idt);
        return;
    }
    insertSortedTag(&np->child, nt->info);
    insertSortedPage(&nt->child, idp);
    TandaiUbah(J);
    printf("Tag berhasil dipasang.\n");
}

void CMDUntag(Jurnal *J) {
    AddressMultiListPage np;
    AddressMultiListTag nt;
    int idp, idt;
    if (!TanyaHalamanTag(J, &np, &idp)) return;
    if (!TanyaTagKatalog(J, &nt, &idt)) return;
    if (searchTag(np->child, nt->info) == NULL) {
        printf("Halaman P%d tidak memiliki tag T%d.\n", idp, idt);
        return;
    }
    deleteAtTag(&np->child, nt->info);
    deleteAtPage(&nt->child, idp);
    TandaiUbah(J);
    printf("Tag berhasil dilepas.\n");
}

void CMDTagDelete(Jurnal *J) {
    AddressMultiListTag nt;
    AddresslistPage p;
    AddressMultiListPage np;
    char pesan[1100];
    bool ya;
    int idt;
    if (!TanyaTagKatalog(J, &nt, &idt)) return;
    snprintf(pesan, sizeof(pesan), "Hapus tag %s dari katalog dan seluruh halaman? (Y/N)", nt->info.kata);
    if (TanyaYN(pesan, &ya) != InputOK) return;
    if (!ya) {
        printf("Penghapusan tag dibatalkan.\n");
        return;
    }
    for (p = nt->child; p != NULL; p = p->next) {
        np = searchMLPageByID(J->pageTags, p->info);
        if (np != NULL) deleteAtTag(&np->child, nt->info);
    }
    TrieHapus(&J->trieTag, nt->info.kata, nt->info.id);
    deleteAtMLTag(&J->tagPages, nt->info);   // sekaligus membebaskan daftar halamannya
    TandaiUbah(J);
    printf("Tag berhasil dihapus.\n");
}

void CMDTagList(Jurnal *J) {
    AddressMultiListTag p = J->tagPages;
    if (p == NULL) {
        printf("Belum ada tag.\n");
        return;
    }
    for (; p != NULL; p = p->next)
        printf("[T%d] %s - %d halaman\n", p->info.id, p->info.kata, lengthListPage(p->child));
}

void CMDPageTags(Jurnal *J) {
    AddressMultiListPage np;
    AddresslistTag t;
    int idp;
    if (!TanyaHalamanTag(J, &np, &idp)) return;
    if (np->child == NULL) {
        printf("Halaman P%d belum memiliki tag.\n", idp);
        return;
    }
    for (t = np->child; t != NULL; t = t->next)   // terurut menurut nama
        printf("[T%d] %s\n", t->info.id, t->info.kata);
}

void CMDTagPages(Jurnal *J) {
    AddressMultiListTag nt;
    AddresslistPage p;
    AddressPage h;
    int idt;
    if (!TanyaTagKatalog(J, &nt, &idt)) return;
    if (nt->child == NULL) {
        printf("Tag T%d belum dipakai halaman mana pun.\n", idt);
        return;
    }
    for (p = nt->child; p != NULL; p = p->next) {   // terurut menurut ID
        h = CariHalamanJurnal(J, p->info);
        printf("[P%d] %s\n", p->info, h != NULL ? h->title.tab : "(tidak ditemukan)");
    }
}