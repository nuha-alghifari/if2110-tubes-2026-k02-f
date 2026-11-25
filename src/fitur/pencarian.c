#include <stdio.h>
#include <stdlib.h>
#include "pencarian.h"
#include "input.h"

// true jika a sebelum b.
static bool Lebih(const InfoHalaman *a, const InfoHalaman *b, KriteriaUrut kriteria, ArahUrut arah) {
    int c;

    if (kriteria == UrutJudul) {
        c = BandingKataTanpaKapital(&a->judul, &b->judul);
        if (c != 0) {
            return (arah == UrutNaik) ? (c < 0) : (c > 0);
        }
        return a->id < b->id;
    }
    return (arah == UrutNaik) ? (a->id < b->id) : (a->id > b->id);
}

void UrutkanHalaman(InfoHalaman *daftar, int n, KriteriaUrut kriteria, ArahUrut arah) {
    InfoHalaman kunci;
    int i, j;

    for (i = 1; i < n; i++) {
        kunci = daftar[i];
        j = i - 1;
        while (j >= 0 && Lebih(&kunci, &daftar[j], kriteria, arah)) {
            daftar[j + 1] = daftar[j];
            j--;
        }
        daftar[j + 1] = kunci;
    }
}

int CariHalaman(const InfoHalaman *daftar, int n, const Kata *kunci, InfoHalaman *hasil) {
    int jumlah = 0;
    int i;

    for (i = 0; i < n; i++) {
        if (KataMengandung(&daftar[i].judul, kunci)) {
            hasil[jumlah] = daftar[i];
            jumlah++;
        }
    }
    UrutkanHalaman(hasil, jumlah, UrutID, UrutNaik);
    return jumlah;
}

bool KataKeKriteria(const Kata *k, KriteriaUrut *kriteria) {
    if (KataSamaC(k, "ID")) {
        *kriteria = UrutID;
        return true;
    }
    if (KataSamaC(k, "TITLE")) {
        *kriteria = UrutJudul;
        return true;
    }
    return false;
}

bool KataKeArah(const Kata *k, ArahUrut *arah) {
    if (KataSamaC(k, "ASC")) {
        *arah = UrutNaik;
        return true;
    }
    if (KataSamaC(k, "DESC")) {
        *arah = UrutTurun;
        return true;
    }
    return false;
}

void CetakInfoHalaman(const InfoHalaman *h) {
    printf("[P%d] %s (parent P%d)\n", h->id, h->judul.tab, h->idParent);
}

static void IsiDaftarHalaman(AddressPage node, InfoHalaman *daftar, int *idx) {
    int id = 0;
    int idParent = 0;

    if (node == NULL) return;
    KataKeID(&node->pageId, 'P', &id);
    if (node->parent != NULL) {
        KataKeID(&node->parent->pageId, 'P', &idParent);
    }
    daftar[*idx].id = id;
    daftar[*idx].idParent = idParent;
    daftar[*idx].judul = node->title;
    (*idx)++;
    IsiDaftarHalaman(node->firstChild, daftar, idx);
    IsiDaftarHalaman(node->nextSibling, daftar, idx);
}

int KumpulkanHalaman(const Jurnal *J, InfoHalaman **daftar) {
    int n;
    int idx = 0;

    *daftar = NULL;
    if (J == NULL || J->root == NULL) return 0;
    n = HitungHalaman(J);
    if (n <= 0) return 0;

    *daftar = (InfoHalaman *) malloc(sizeof(InfoHalaman) * n);
    if (*daftar == NULL) return -1;

    IsiDaftarHalaman(J->root->firstChild, *daftar, &idx);
    return idx;
}

void CMDSearch(Jurnal *J) {
    Kata kunci;
    InfoHalaman *semua;
    InfoHalaman *hasil;
    int n, jumlah, i;

    if (TanyaTidakKosong("Masukkan kata pencarian:", &kunci) != InputOK) return;

    n = KumpulkanHalaman(J, &semua);
    if (n < 0) {
        printf("Memori tidak cukup.\n");
        return;
    }
    if (n == 0) {
        printf("Halaman tidak ditemukan.\n");
        return;
    }

    hasil = (InfoHalaman *) malloc(sizeof(InfoHalaman) * n);
    if (hasil == NULL) {
        printf("Memori tidak cukup.\n");
        free(semua);
        return;
    }

    jumlah = CariHalaman(semua, n, &kunci, hasil);
    if (jumlah == 0) {
        printf("Halaman tidak ditemukan.\n");
    }
    for (i = 0; i < jumlah; i++) {
        CetakInfoHalaman(&hasil[i]);
    }
    free(hasil);
    free(semua);
}

void CMDPages(Jurnal *J) {
    Kata kataKriteria, kataArah;
    KriteriaUrut kriteria;
    ArahUrut arah;
    InfoHalaman *semua;
    int n, i;

    if (Tanya("Urutkan berdasarkan (ID/TITLE):", &kataKriteria) != InputOK) return;
    if (!KataKeKriteria(&kataKriteria, &kriteria)) {
        printf("Pilihan harus ID atau TITLE.\n");
        return;
    }
    if (Tanya("Urutan (ASC/DESC):", &kataArah) != InputOK) return;
    if (!KataKeArah(&kataArah, &arah)) {
        printf("Urutan harus ASC atau DESC.\n");
        return;
    }

    n = KumpulkanHalaman(J, &semua);
    if (n < 0) {
        printf("Memori tidak cukup.\n");
        return;
    }
    if (n == 0) {
        printf("Belum ada halaman.\n");
        return;
    }

    UrutkanHalaman(semua, n, kriteria, arah);
    for (i = 0; i < n; i++) {
        CetakInfoHalaman(&semua[i]);
    }
    free(semua);
}
