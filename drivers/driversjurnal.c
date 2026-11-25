#include <stdio.h>
#include "jurnal.h"
#include "tag.h"

static int total = 0;
static int gagal = 0;

// isi jurnal: P1 anak P0, P2 anak P1, 1 blok di P2, 1 tag, 1 link
static void IsiJurnal(Jurnal *j){
    Kata id, judul, teks;
    AddressPage p1, p2;
    Tag t;

    KataDariC(&id, "P1"); KataDariC(&judul, "Satu");
    p1 = createPage(id, judul);
    p1->parent = j->root;
    j->root->firstChild = p1;
    AmbilIDHalaman(j);

    KataDariC(&id, "P2"); KataDariC(&judul, "Dua");
    p2 = createPage(id, judul);
    p2->parent = p1;
    p1->firstChild = p2;
    AmbilIDHalaman(j);

    KataDariC(&teks, "isi blok");
    TambahBlok(&p2->blocks, AmbilIDBlok(j), BlokText, &teks, 1);

    KataDariC(&teks, "rencana");
    t = copyKata(teks);
    insertLastMLTag(&j->tagPages, t);
    insertLastMLPage(&j->pageTags, 1);
    insertLinkedListTag(&j->tagPages, t, 1);
    insertLinkedListPage(&j->pageTags, 1, t);

    insertVertex(&j->linkKeluar, 1);
    insertVertex(&j->linkKeluar, 2);
    addEdge(&j->linkKeluar, 1, 2);
    insertVertex(&j->linkMasuk, 1);
    insertVertex(&j->linkMasuk, 2);
    addEdge(&j->linkMasuk, 2, 1);
    TandaiUbah(j);
}

static void cek(bool kondisi, const char *nama){
    total++;
    if (!kondisi){
        gagal++;
        printf("GAGAL: %s\n", nama);
    }
}

int main(void){
    Jurnal a, b;

    InitJurnal(&a);
    cek(a.nextPageId == 1 && a.nextBlockId == 1 && a.nextTagId == 1, "counter awal 1");
    cek(!a.perubahan, "Perubahan awal false");

    // ID naik satu-satu dan tidak dipakai ulang
    cek(AmbilIDHalaman(&a) == 1, "halaman pertama P1");
    cek(AmbilIDHalaman(&a) == 2, "halaman kedua P2");
    cek(AmbilIDBlok(&a) == 1, "blok pertama B1");
    cek(AmbilIDTag(&a) == 1, "tag pertama T1");
    cek(a.nextPageId == 3, "counter halaman jadi 3");

    // perubahan
    TandaiUbah(&a);
    cek(a.perubahan, "TandaiUbah");
    TandaiTersimpan(&a);
    cek(!a.perubahan, "TandaiTersimpan");

    // tukar
    InitJurnal(&b);
    AmbilIDHalaman(&b);
    AmbilIDHalaman(&b);
    AmbilIDHalaman(&b);
    TandaiUbah(&b);
    TukarJurnal(&a, &b);
    cek(a.nextPageId == 4 && a.perubahan, "isi a setelah tukar");
    cek(b.nextPageId == 3 && !b.perubahan, "isi b setelah tukar");

    // kosongkan mengembalikan ke awal
    KosongkanJurnal(&a);
    cek(a.nextPageId == 1 && !a.perubahan, "KosongkanJurnal");

        HapusJurnal(&b);  // b dari tes di atas tidak dipakai lagi
    cek(a.root != NULL, "root P0 dibuat");
    cek(CariHalamanJurnal(&a, 0) == a.root, "CariHalamanJurnal P0");
    cek(CariHalamanJurnal(&a, 1) == NULL, "halaman belum ada");
    cek(CariHalamanJurnal(&a, -1) == NULL, "id negatif ditolak");

    IsiJurnal(&a);
    cek(CariHalamanJurnal(&a, 2) != NULL && KataSamaC(&CariHalamanJurnal(&a, 2)->title, "Dua"), "CariHalamanJurnal P2");
    cek(CariPemilikBlok(&a, 1) == CariHalamanJurnal(&a, 2), "CariPemilikBlok B1 = P2");
    cek(CariPemilikBlok(&a, 9) == NULL, "CariPemilikBlok blok tidak ada");
    cek(a.linkKeluar.length_vertex == 2 && a.tagPages != NULL, "graph dan tag terisi");

    KosongkanJurnal(&a);
    cek(a.root != NULL && a.root->firstChild == NULL, "NEW menyisakan P0 kosong");
    cek(a.pageTags == NULL && a.tagPages == NULL, "NEW mengosongkan multilist");
    cek(a.linkKeluar.length_vertex == 0 && a.linkMasuk.length_vertex == 0, "NEW mengosongkan graph");
    cek(a.nextPageId == 1 && a.nextBlockId == 1 && a.nextTagId == 1 && !a.perubahan, "NEW reset counter");
    cek(CariHalamanJurnal(&a, 1) == NULL, "halaman lama hilang setelah NEW");

    InitJurnal(&b);
    IsiJurnal(&b);
    TukarJurnal(&a, &b);
    cek(CariHalamanJurnal(&a, 1) != NULL, "isi pindah setelah tukar");
    HapusJurnal(&b);
    cek(b.root == NULL, "HapusJurnal mengosongkan root");

    HapusJurnal(&a);

    printf("Jurnal driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}