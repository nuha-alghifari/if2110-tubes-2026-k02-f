#include <stdio.h>
#include "linkbacklink.h"
#include "queue.h"

#define MAX_BFS 1000

static bool TanyaHalamanLink(Jurnal *j, const char *pertanyaan, int *id) {
    if (TanyaID(pertanyaan, 'P', id) != InputOK) return false;
    if (*id == 0) {
        printf("P0 (Workspace) bukan halaman, tidak dapat dipakai pada link.\n");
        return false;
    }
    if (CariHalamanJurnal(j, *id) == NULL || SearchVertexIdx(j->linkKeluar, *id) == INVALID_IDX) {
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
    for (p = j->linkKeluar.node[i].Edge; p != NULL; p = p->next)
        deleteEdge(&j->linkMasuk, p->info, idHalaman);
    k = SearchVertexIdx(j->linkMasuk, idHalaman);
    if (k != INVALID_IDX)
        for (p = j->linkMasuk.node[k].Edge; p != NULL; p = p->next)
            deleteEdge(&j->linkKeluar, p->info, idHalaman);
    deleteAllPage(&j->linkKeluar.node[i].Edge);
    if (k != INVALID_IDX) deleteAllPage(&j->linkMasuk.node[k].Edge);
}

void CMDLinkPath(Jurnal *j) {
    int sumber, tujuan;
    int iSumber, iTujuan, i;
    int parent[MAX_BFS];
    bool dikunjungi[MAX_BFS];
    Queue q;
    int cur;
    bool ketemu;

    if (!TanyaHalamanLink(j, "Masukkan ID halaman sumber:", &sumber)) return;
    if (!TanyaHalamanLink(j, "Masukkan ID halaman tujuan:", &tujuan)) return;

    iSumber = SearchVertexIdx(j->linkKeluar, sumber);
    iTujuan = SearchVertexIdx(j->linkKeluar, tujuan);

    for (i = 0; i < MAX_BFS; i++) {
        dikunjungi[i] = false;
        parent[i] = INVALID_IDX;
    }

    CreateQueue(&q);
    dikunjungi[iSumber] = true;
    Enqueue(&q, iSumber);
    ketemu = (iSumber == iTujuan);

    while (!ketemu && Dequeue(&q, &cur)) {
        AddresslistPage p;
        for (p = j->linkKeluar.node[cur].Edge; p != NULL && !ketemu; p = p->next) {
            int n = SearchVertexIdx(j->linkKeluar, p->info);
            if (n == INVALID_IDX || dikunjungi[n]) continue;
            dikunjungi[n] = true;
            parent[n] = cur;
            if (n == iTujuan) {
                ketemu = true;
            } else {
                Enqueue(&q, n);
            }
        }
    }
    DeleteAllQueue(&q);

    if (!ketemu) {
        printf("Tidak ada jalur dari P%d ke P%d.\n", sumber, tujuan);
        return;
    }

    {
        int jalur[MAX_BFS];
        int panjang = 0;
        int k;
        for (cur = iTujuan; cur != INVALID_IDX; cur = parent[cur]) {
            jalur[panjang++] = cur;
        }
        for (k = panjang - 1; k >= 0; k--) {
            int id = j->linkKeluar.node[jalur[k]].info;
            AddressPage h = CariHalamanJurnal(j, id);
            printf("P%d %s", id, h != NULL ? h->title.tab : "(tidak ditemukan)");
            if (k > 0) printf(" -> ");
        }
        printf("\n");
        printf("Panjang: %d link\n", panjang - 1);
    }
}