#include <stdio.h>
#include "linkbacklink.h"

static bool TanyaHalamanLink(Jurnal *j, const char *pertanyaan, int *id) {
    if (TanyaID(pertanyaan, 'P', id) != InputOK) return false;
    if (SearchVertexIdx(j->linkKeluar, *id) == INVALID_IDX) {
        printf("Halaman P%d tidak ditemukan.\n", *id);
        return false;
    }
    return true;
}

static void CetakDaftarHalaman(Jurnal *j, AddresslistPage p, const char *kosong) {
    if (p == NULL) {
        printf("%s\n", kosong);
        return;
    }
    for (; p != NULL; p = p->next) {   // sudah terurut menurut ID
        AddressPage h = CariHalamanJurnal(j, p->info);
        printf("[P%d] %s\n", p->info, h != NULL ? h->title.tab : "(tidak ditemukan)");
    }
}

void CMDLink(Jurnal *j) {
    int a, b, ia;
    if (!TanyaHalamanLink(j, "Masukkan ID halaman sumber:", &a)) return;
    if (!TanyaHalamanLink(j, "Masukkan ID halaman tujuan:", &b)) return;
    if (a == b) {
        printf("Halaman tidak dapat menautkan dirinya sendiri.\n");
        return;
    }
    ia = SearchVertexIdx(j->linkKeluar, a);
    if (searchPage(j->linkKeluar.node[ia].Edge, b) != NULL) {
        printf("Link P%d -> P%d sudah ada.\n", a, b);
        return;
    }
    addEdge(&j->linkKeluar, a, b);
    addEdge(&j->linkMasuk, b, a);
    TandaiUbah(j);
    printf("Link berhasil ditambahkan.\n");
}

void CMDUnlink(Jurnal *j) {
    int a, b, ia;
    if (!TanyaHalamanLink(j, "Masukkan ID halaman sumber:", &a)) return;
    if (!TanyaHalamanLink(j, "Masukkan ID halaman tujuan:", &b)) return;
    ia = SearchVertexIdx(j->linkKeluar, a);
    if (searchPage(j->linkKeluar.node[ia].Edge, b) == NULL) {
        printf("Link P%d -> P%d tidak ditemukan.\n", a, b);
        return;
    }
    deleteEdge(&j->linkKeluar, a, b);
    deleteEdge(&j->linkMasuk, b, a);
    TandaiUbah(j);
    printf("Link berhasil dihapus.\n");
}

void CMDLinks(Jurnal *j) {
    int id, i;
    if (!TanyaHalamanLink(j, "Masukkan ID halaman:", &id)) return;
    i = SearchVertexIdx(j->linkKeluar, id);
    CetakDaftarHalaman(j, j->linkKeluar.node[i].Edge, "Belum ada halaman yang dirujuk.");
}

void CMDBacklinks(Jurnal *j) {
    int id, i;
    if (!TanyaHalamanLink(j, "Masukkan ID halaman:", &id)) return;
    i = SearchVertexIdx(j->linkMasuk, id);
    CetakDaftarHalaman(j, j->linkMasuk.node[i].Edge, "Belum ada halaman yang merujuk.");
}

void LinkHapusHalaman(Jurnal *j, int idHalaman) {
    int i = SearchVertexIdx(j->linkKeluar, idHalaman);
    int k;
    AddresslistPage p;
    if (i == INVALID_IDX) return;
    // link keluar halaman ini: hapus dari backlink tujuan
    for (p = j->linkKeluar.node[i].Edge; p != NULL; p = p->next)
        deleteEdge(&j->linkMasuk, p->info, idHalaman);
    // link masuk ke halaman ini: hapus dari link keluar sumber
    k = SearchVertexIdx(j->linkMasuk, idHalaman);
    if (k != INVALID_IDX)
        for (p = j->linkMasuk.node[k].Edge; p != NULL; p = p->next)
            deleteEdge(&j->linkKeluar, p->info, idHalaman);
    deleteAllPage(&j->linkKeluar.node[i].Edge);
    if (k != INVALID_IDX) deleteAllPage(&j->linkMasuk.node[k].Edge);
}