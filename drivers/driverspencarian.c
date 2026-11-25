#include <stdio.h>
#include <stdlib.h>
#include "kata.h"
#include "pencarian.h"

static int total = 0;
static int gagal = 0;

static void cek(bool kondisi, const char *nama) {
    total++;
    if (!kondisi) {
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

static Kata BuatKata(const char *s) {
    Kata k;
    KataDariC(&k, s);
    return k;
}

static InfoHalaman BuatInfo(int id, int idParent, const char *judul) {
    InfoHalaman h;
    h.id = id;
    h.idParent = idParent;
    h.judul = BuatKata(judul);
    return h;
}

// true jika urutan ID di daftar sama dengan harapan, contoh "1,3,2"
static bool urutanSama(const InfoHalaman *daftar, int n, const char *harapan) {
    char buf[128];
    int len = 0;
    int i;
    Kata hasil;

    buf[0] = '\0';
    for (i = 0; i < n; i++) {
        len += sprintf(buf + len, "%s%d", i == 0 ? "" : ",", daftar[i].id);
    }
    hasil = BuatKata(buf);
    return KataSamaC(&hasil, harapan);
}

static void ujiCari(void) {
    InfoHalaman sample[3];
    InfoHalaman angka[3];
    InfoHalaman hasil[3];
    Kata kunci;
    int n;

    sample[0] = BuatInfo(3, 1, "Portal Kampus");
    sample[1] = BuatInfo(1, 0, "Jurnal Anomali");
    sample[2] = BuatInfo(2, 0, "Rencana Mebel");

    kunci = BuatKata("PORTAL");
    n = CariHalaman(sample, 3, &kunci, hasil);
    cek(n == 1 && hasil[0].id == 3 && hasil[0].idParent == 1, "cari tanpa beda kapital");
    kunci = BuatKata("portal");
    cek(CariHalaman(sample, 3, &kunci, hasil) == 1, "cari huruf kecil");
    kunci = BuatKata("kamp");
    cek(CariHalaman(sample, 3, &kunci, hasil) == 1 && hasil[0].id == 3, "cari sebagian judul");
    kunci = BuatKata("a");
    n = CariHalaman(sample, 3, &kunci, hasil);
    cek(n == 3 && urutanSama(hasil, n, "1,2,3"), "hasil urut ID naik");
    kunci = BuatKata("xyz");
    cek(CariHalaman(sample, 3, &kunci, hasil) == 0, "tidak ada kecocokan");
    kunci = BuatKata("Portal Kampus Timur");
    cek(CariHalaman(sample, 3, &kunci, hasil) == 0, "kata lebih panjang dari judul");
    kunci = BuatKata("a");
    cek(CariHalaman(sample, 0, &kunci, hasil) == 0, "cari di daftar kosong");

    angka[0] = BuatInfo(10, 0, "x satu");
    angka[1] = BuatInfo(2, 0, "x dua");
    angka[2] = BuatInfo(9, 0, "x tiga");
    kunci = BuatKata("x");
    n = CariHalaman(angka, 3, &kunci, hasil);
    cek(n == 3 && urutanSama(hasil, n, "2,9,10"), "urutan angka, bukan teks");
    cek(angka[0].id == 10 && angka[1].id == 2, "daftar asli tidak berubah");
}

static void ujiUrut(void) {
    InfoHalaman d[4];
    InfoHalaman sample[3];
    InfoHalaman kapital[3];

    d[0] = BuatInfo(3, 0, "c");
    d[1] = BuatInfo(1, 0, "a");
    d[2] = BuatInfo(10, 0, "b");
    d[3] = BuatInfo(2, 0, "d");
    UrutkanHalaman(d, 4, UrutID, UrutNaik);
    cek(urutanSama(d, 4, "1,2,3,10"), "urut ID naik");
    UrutkanHalaman(d, 4, UrutID, UrutTurun);
    cek(urutanSama(d, 4, "10,3,2,1"), "urut ID turun");

    sample[0] = BuatInfo(1, 0, "Jurnal Anomali");
    sample[1] = BuatInfo(2, 0, "Rencana Mebel");
    sample[2] = BuatInfo(3, 1, "Portal Kampus");
    UrutkanHalaman(sample, 3, UrutJudul, UrutNaik);
    cek(urutanSama(sample, 3, "1,3,2"), "urut judul naik");
    cek(sample[1].idParent == 1 && KataSamaC(&sample[1].judul, "Portal Kampus"), "isi halaman ikut terurut");
    UrutkanHalaman(sample, 3, UrutJudul, UrutTurun);
    cek(urutanSama(sample, 3, "2,3,1"), "urut judul turun");

    kapital[0] = BuatInfo(1, 0, "Banana");
    kapital[1] = BuatInfo(5, 0, "apel");
    kapital[2] = BuatInfo(2, 0, "Apel");
    UrutkanHalaman(kapital, 3, UrutJudul, UrutNaik);
    cek(urutanSama(kapital, 3, "2,5,1"), "judul sama beda kapital, ID kecil dulu (naik)");
    UrutkanHalaman(kapital, 3, UrutJudul, UrutTurun);
    cek(urutanSama(kapital, 3, "1,2,5"), "judul sama beda kapital, ID kecil dulu (turun)");

    UrutkanHalaman(d, 0, UrutID, UrutNaik);
    UrutkanHalaman(d, 1, UrutJudul, UrutTurun);
    cek(true, "urut n=0 dan n=1 tidak error");
}

static void ujiKata(void) {
    KriteriaUrut kriteria;
    ArahUrut arah;
    Kata k;

    k = BuatKata("ID");
    cek(KataKeKriteria(&k, &kriteria) && kriteria == UrutID, "kriteria ID");
    k = BuatKata("TITLE");
    cek(KataKeKriteria(&k, &kriteria) && kriteria == UrutJudul, "kriteria TITLE");
    k = BuatKata("id");
    cek(!KataKeKriteria(&k, &kriteria), "kriteria huruf kecil ditolak");
    k = BuatKata("");
    cek(!KataKeKriteria(&k, &kriteria), "kriteria kosong ditolak");
    k = BuatKata("NAMA");
    cek(!KataKeKriteria(&k, &kriteria), "kriteria tidak dikenal ditolak");

    k = BuatKata("ASC");
    cek(KataKeArah(&k, &arah) && arah == UrutNaik, "arah ASC");
    k = BuatKata("DESC");
    cek(KataKeArah(&k, &arah) && arah == UrutTurun, "arah DESC");
    k = BuatKata("asc");
    cek(!KataKeArah(&k, &arah), "arah huruf kecil ditolak");
    k = BuatKata("NAIK");
    cek(!KataKeArah(&k, &arah), "arah tidak dikenal ditolak");
}

static void ujiKumpul(void) {
    Jurnal j;
    InfoHalaman *daftar;
    InfoHalaman hasil[3];
    Kata kunci;
    char pesan[Max_Pesan];
    int n, m;

    InitJurnal(&j);
    n = KumpulkanHalaman(&j, &daftar);
    cek(n == 0 && daftar == NULL, "kumpul jurnal kosong");
    HapusJurnal(&j);

    InitJurnal(&j);
    cek(MuatJurnal(&j, "sample-data/jurnal", pesan, Max_Pesan), "muat sample untuk pencarian");
    n = KumpulkanHalaman(&j, &daftar);
    cek(n == 3 && daftar != NULL, "kumpul 3 halaman tanpa P0");

    kunci = BuatKata("PORTAL");
    m = CariHalaman(daftar, n, &kunci, hasil);
    cek(m == 1 && hasil[0].id == 3 && hasil[0].idParent == 1, "cari dari jurnal termuat");

    UrutkanHalaman(daftar, n, UrutJudul, UrutNaik);
    cek(urutanSama(daftar, n, "1,3,2"), "urut dari jurnal termuat");

    free(daftar);
    HapusJurnal(&j);
}

int main(void) {
    InfoHalaman h;

    ujiCari();
    ujiUrut();
    ujiKata();
    ujiKumpul();

    printf("contoh\n");
    h = BuatInfo(3, 1, "Portal Kampus");
    CetakInfoHalaman(&h);
    h = BuatInfo(1, 0, "Jurnal Anomali");
    CetakInfoHalaman(&h);
    printf("selesai\n");

    printf("Pencarian driver: %d/%d lulus\n", total - gagal, total);
    return gagal == 0 ? 0 : 1;
}
