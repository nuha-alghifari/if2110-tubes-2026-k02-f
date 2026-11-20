#include <stdlib.h>
#include "block.h"

// Lepas blok dari list tanpa membebaskannya. NULL jika id tidak ada.
static AddressBlok LepasBlok(ListBlok *l, int id) {
    AddressBlok prev = NULL;
    AddressBlok p = l->first;

    while (p != NULL && p->id != id) {
        prev = p;
        p = p->next;
    }
    if (p == NULL) return NULL;

    if (prev == NULL) {
        l->first = p->next;
    } else {
        prev->next = p->next;
    }
    p->next = NULL;
    l->jumlah--;
    return p;
}

// Sisipkan blok di posisi 1..jumlah+1.
static void SisipBlok(ListBlok *l, AddressBlok p, int posisi) {
    AddressBlok q;
    int i;

    if (posisi == 1) {
        p->next = l->first;
        l->first = p;
    } else {
        q = l->first;
        for (i = 1; i < posisi - 1; i++) {
            q = q->next;
        }
        p->next = q->next;
        q->next = p;
    }
    l->jumlah++;
}

void BuatListBlok(ListBlok *l) {
    l->first = NULL;
    l->jumlah = 0;
}

bool ListBlokKosong(const ListBlok *l) {
    return l->first == NULL;
}

int JumlahBlok(const ListBlok *l) {
    return l->jumlah;
}

int JumlahChecklist(const ListBlok *l) {
    AddressBlok p;
    int n = 0;

    for (p = l->first; p != NULL; p = p->next) {
        if (p->tipe == BlokCheck) n++;
    }
    return n;
}

int JumlahChecklistSelesai(const ListBlok *l) {
    AddressBlok p;
    int n = 0;

    for (p = l->first; p != NULL; p = p->next) {
        if (p->tipe == BlokCheck && p->selesai) n++;
    }
    return n;
}

AddressBlok CariBlok(const ListBlok *l, int id) {
    AddressBlok p = l->first;

    while (p != NULL && p->id != id) {
        p = p->next;
    }
    return p;
}

int PosisiBlok(const ListBlok *l, int id) {
    AddressBlok p;
    int posisi = 1;

    for (p = l->first; p != NULL; p = p->next) {
        if (p->id == id) return posisi;
        posisi++;
    }
    return 0;
}

StatusBlok TambahBlok(ListBlok *l, int id, TipeBlok tipe, const Kata *teks, int posisi) {
    AddressBlok baru;

    if (teks->length < 1) return BlokTeksKosong;
    if (posisi < 1 || posisi > l->jumlah + 1) return BlokPosisiDiLuarRentang;

    baru = (AddressBlok) malloc(sizeof(Block));
    if (baru == NULL) return BlokGagalAlokasi;

    baru->id = id;
    baru->tipe = tipe;
    baru->teks = *teks;
    baru->selesai = false;
    SisipBlok(l, baru, posisi);
    return BlokOK;
}

StatusBlok UbahTeksBlok(ListBlok *l, int id, const Kata *teksBaru) {
    AddressBlok p = CariBlok(l, id);

    if (p == NULL) return BlokTidakDitemukan;
    if (teksBaru->length < 1) return BlokTeksKosong;

    p->teks = *teksBaru;
    return BlokOK;
}

StatusBlok PindahBlok(ListBlok *l, int id, int posisiBaru) {
    AddressBlok p;

    if (CariBlok(l, id) == NULL) return BlokTidakDitemukan;
    if (posisiBaru < 1 || posisiBaru > l->jumlah) return BlokPosisiDiLuarRentang;

    p = LepasBlok(l, id);
    SisipBlok(l, p, posisiBaru);
    return BlokOK;
}

StatusBlok HapusBlok(ListBlok *l, int id) {
    AddressBlok p = LepasBlok(l, id);

    if (p == NULL) return BlokTidakDitemukan;
    free(p);
    return BlokOK;
}

StatusBlok UbahStatusBlok(ListBlok *l, int id, bool selesai) {
    AddressBlok p = CariBlok(l, id);

    if (p == NULL) return BlokTidakDitemukan;
    if (p->tipe != BlokCheck) return BlokBukanChecklist;

    p->selesai = selesai;
    return BlokOK;
}

void HapusSemuaBlok(ListBlok *l) {
    AddressBlok p = l->first;
    AddressBlok next;

    while (p != NULL) {
        next = p->next;
        free(p);
        p = next;
    }
    BuatListBlok(l);
}

bool KataKeTipeBlok(const Kata *k, TipeBlok *tipe) {
    if (KataSamaC(k, "TEXT")) {
        *tipe = BlokText;
        return true;
    }
    if (KataSamaC(k, "CHECK")) {
        *tipe = BlokCheck;
        return true;
    }
    return false;
}
