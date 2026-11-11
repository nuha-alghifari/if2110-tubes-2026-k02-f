#include <stdio.h>
#include "jurnal.h"

static int total = 0;
static int gagal = 0;

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

    printf("Jurnal driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}