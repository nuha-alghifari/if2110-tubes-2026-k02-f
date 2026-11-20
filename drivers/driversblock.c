#include <stdio.h>
#include "kata.h"
#include "block.h"
#include "block_cmd.h"

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

// true jika urutan ID di list sama dengan harapan
static bool urutanSama(const ListBlok *l, const char *harapan) {
    char buf[128];
    int len = 0;
    AddressBlok p;
    Kata hasil;

    buf[0] = '\0';
    for (p = l->first; p != NULL; p = p->next) {
        len += sprintf(buf + len, "%s%d", len == 0 ? "" : ",", p->id);
    }
    hasil = BuatKata(buf);
    return KataSamaC(&hasil, harapan);
}

static bool teksSama(const ListBlok *l, int id, const char *harapan) {
    AddressBlok p = CariBlok(l, id);
    return p != NULL && KataSamaC(&p->teks, harapan);
}

static void ujiTambah(void) {
    ListBlok l;
    Kata teks;

    BuatListBlok(&l);
    cek(ListBlokKosong(&l) && JumlahBlok(&l) == 0, "list baru kosong");

    teks = BuatKata("Portal ditemukan.");
    cek(TambahBlok(&l, 1, BlokText, &teks, 1) == BlokOK, "tambah di list kosong");
    teks = BuatKata("Bawa kompas");
    cek(TambahBlok(&l, 2, BlokCheck, &teks, 2) == BlokOK, "tambah di posisi n+1");
    teks = BuatKata("Periksa");
    cek(TambahBlok(&l, 3, BlokCheck, &teks, 1) == BlokOK, "tambah di posisi 1");
    teks = BuatKata("Catatan");
    cek(TambahBlok(&l, 4, BlokText, &teks, 3) == BlokOK, "tambah di tengah");
    cek(urutanSama(&l, "3,1,4,2"), "urutan setelah tambah");
    cek(JumlahBlok(&l) == 4, "jumlah setelah tambah");

    cek(TambahBlok(&l, 5, BlokText, &teks, 0) == BlokPosisiDiLuarRentang, "posisi 0 ditolak");
    cek(TambahBlok(&l, 5, BlokText, &teks, -1) == BlokPosisiDiLuarRentang, "posisi negatif ditolak");
    cek(TambahBlok(&l, 5, BlokText, &teks, 6) == BlokPosisiDiLuarRentang, "posisi n+2 ditolak");
    teks = BuatKata("");
    cek(TambahBlok(&l, 5, BlokText, &teks, 1) == BlokTeksKosong, "teks kosong ditolak");
    cek(JumlahBlok(&l) == 4 && urutanSama(&l, "3,1,4,2"), "gagal tambah tidak mengubah list");

    teks = BuatKata("Ekor");
    cek(TambahBlok(&l, 5, BlokText, &teks, 5) == BlokOK, "tambah di posisi n+1 lagi");
    cek(urutanSama(&l, "3,1,4,2,5"), "urutan akhir tambah");

    cek(CariBlok(&l, 4) != NULL && CariBlok(&l, 99) == NULL, "cari blok");
    cek(PosisiBlok(&l, 4) == 3 && PosisiBlok(&l, 99) == 0, "posisi blok");
    cek(CariBlok(&l, 2)->selesai == false, "checklist baru berstatus TODO");

    HapusSemuaBlok(&l);
}

static void ujiUbah(void) {
    ListBlok l;
    Kata teks;

    BuatListBlok(&l);
    teks = BuatKata("Awal");
    TambahBlok(&l, 1, BlokText, &teks, 1);
    teks = BuatKata("Cek");
    TambahBlok(&l, 2, BlokCheck, &teks, 2);

    teks = BuatKata("Baru");
    cek(UbahTeksBlok(&l, 1, &teks) == BlokOK && teksSama(&l, 1, "Baru"), "ubah teks");
    cek(UbahTeksBlok(&l, 99, &teks) == BlokTidakDitemukan, "ubah teks ID tidak ada");
    teks = BuatKata("");
    cek(UbahTeksBlok(&l, 1, &teks) == BlokTeksKosong, "ubah teks kosong ditolak");
    cek(teksSama(&l, 1, "Baru"), "teks tetap saat ubah gagal");

    cek(UbahStatusBlok(&l, 2, true) == BlokOK && CariBlok(&l, 2)->selesai, "tandai DONE");
    cek(UbahStatusBlok(&l, 2, false) == BlokOK && !CariBlok(&l, 2)->selesai, "kembalikan TODO");
    cek(UbahStatusBlok(&l, 1, true) == BlokBukanChecklist, "TEXT tidak bisa di-CHECK");
    cek(!CariBlok(&l, 1)->selesai, "status TEXT tidak berubah");
    cek(UbahStatusBlok(&l, 99, true) == BlokTidakDitemukan, "status ID tidak ada");

    HapusSemuaBlok(&l);
}

static void ujiHitung(void) {
    ListBlok l;
    Kata teks = BuatKata("x");

    BuatListBlok(&l);
    cek(JumlahChecklist(&l) == 0 && JumlahChecklistSelesai(&l) == 0, "hitung list kosong");

    TambahBlok(&l, 1, BlokText, &teks, 1);
    TambahBlok(&l, 2, BlokCheck, &teks, 2);
    TambahBlok(&l, 3, BlokCheck, &teks, 3);
    TambahBlok(&l, 4, BlokCheck, &teks, 4);
    UbahStatusBlok(&l, 2, true);
    UbahStatusBlok(&l, 4, true);

    cek(JumlahBlok(&l) == 4, "jumlah blok");
    cek(JumlahChecklist(&l) == 3, "jumlah checklist");
    cek(JumlahChecklistSelesai(&l) == 2, "jumlah checklist selesai");

    HapusSemuaBlok(&l);
}

static void ujiPindah(void) {
    ListBlok l;
    Kata teks = BuatKata("x");
    int i;

    BuatListBlok(&l);
    for (i = 1; i <= 5; i++) {
        TambahBlok(&l, i, BlokText, &teks, i);
    }
    cek(urutanSama(&l, "1,2,3,4,5"), "urutan awal pindah");

    cek(PindahBlok(&l, 5, 1) == BlokOK && urutanSama(&l, "5,1,2,3,4"), "pindah ke depan");
    cek(PindahBlok(&l, 5, 5) == BlokOK && urutanSama(&l, "1,2,3,4,5"), "pindah ke belakang");
    cek(PindahBlok(&l, 1, 1) == BlokOK && urutanSama(&l, "1,2,3,4,5"), "pindah ke posisi sama");
    cek(PindahBlok(&l, 1, 3) == BlokOK && urutanSama(&l, "2,3,1,4,5"), "pindah maju");
    cek(PindahBlok(&l, 4, 2) == BlokOK && urutanSama(&l, "2,4,3,1,5"), "pindah mundur");

    cek(PindahBlok(&l, 4, 0) == BlokPosisiDiLuarRentang, "pindah posisi 0 ditolak");
    cek(PindahBlok(&l, 4, 6) == BlokPosisiDiLuarRentang, "pindah posisi n+1 ditolak");
    cek(PindahBlok(&l, 99, 1) == BlokTidakDitemukan, "pindah ID tidak ada");
    cek(urutanSama(&l, "2,4,3,1,5") && JumlahBlok(&l) == 5, "gagal pindah tidak mengubah list");

    HapusSemuaBlok(&l);
    BuatListBlok(&l);
    TambahBlok(&l, 1, BlokText, &teks, 1);
    cek(PindahBlok(&l, 1, 1) == BlokOK && urutanSama(&l, "1"), "pindah blok tunggal");
    HapusSemuaBlok(&l);
}

static void ujiHapus(void) {
    ListBlok l;
    Kata teks = BuatKata("x");
    int i;

    BuatListBlok(&l);
    for (i = 1; i <= 5; i++) {
        TambahBlok(&l, i, BlokText, &teks, i);
    }
    cek(HapusBlok(&l, 1) == BlokOK && urutanSama(&l, "2,3,4,5"), "hapus kepala");
    cek(HapusBlok(&l, 5) == BlokOK && urutanSama(&l, "2,3,4"), "hapus ekor");
    cek(HapusBlok(&l, 3) == BlokOK && urutanSama(&l, "2,4"), "hapus tengah");
    cek(HapusBlok(&l, 99) == BlokTidakDitemukan, "hapus ID tidak ada");
    cek(JumlahBlok(&l) == 2 && PosisiBlok(&l, 4) == 2, "posisi kembali berurutan");
    cek(HapusBlok(&l, 2) == BlokOK && HapusBlok(&l, 4) == BlokOK, "hapus sampai habis");
    cek(ListBlokKosong(&l) && JumlahBlok(&l) == 0, "list kosong setelah hapus semua");

    for (i = 1; i <= 3; i++) {
        TambahBlok(&l, i, BlokText, &teks, i);
    }
    HapusSemuaBlok(&l);
    cek(ListBlokKosong(&l) && JumlahBlok(&l) == 0, "hapus semua blok");
}

static void ujiTipe(void) {
    TipeBlok tipe;
    Kata k;

    k = BuatKata("TEXT");
    cek(KataKeTipeBlok(&k, &tipe) && tipe == BlokText, "tipe TEXT");
    k = BuatKata("CHECK");
    cek(KataKeTipeBlok(&k, &tipe) && tipe == BlokCheck, "tipe CHECK");
    k = BuatKata("text");
    cek(!KataKeTipeBlok(&k, &tipe), "tipe huruf kecil ditolak");
    k = BuatKata("");
    cek(!KataKeTipeBlok(&k, &tipe), "tipe kosong ditolak");
    k = BuatKata("CHECKS");
    cek(!KataKeTipeBlok(&k, &tipe), "tipe tidak dikenal ditolak");
}

static void ujiPerintahAdd(ListBlok *l) {
    Kata tipe, posisi, teks;

    tipe = BuatKata("CHECK");
    posisi = BuatKata("1");
    teks = BuatKata("Periksa koordinat");
    cek(ProsesBlockAdd(l, 2, &tipe, &posisi, &teks), "perintah add CHECK");

    teks = BuatKata("Bawa senter");
    posisi = BuatKata("2");
    cek(ProsesBlockAdd(l, 3, &tipe, &posisi, &teks), "perintah add di akhir");

    tipe = BuatKata("TEXT");
    posisi = BuatKata("1");
    teks = BuatKata("Catatan awal");
    cek(ProsesBlockAdd(l, 4, &tipe, &posisi, &teks), "perintah add TEXT");
    cek(urutanSama(l, "4,2,3"), "urutan setelah add");
}

static void ujiPerintahAddGagal(ListBlok *l) {
    Kata tipe, posisi, teks;

    tipe = BuatKata("NOTE");
    posisi = BuatKata("1");
    teks = BuatKata("x");
    cek(!ProsesBlockAdd(l, 5, &tipe, &posisi, &teks), "add tipe tidak valid");

    tipe = BuatKata("TEXT");
    posisi = BuatKata("abc");
    cek(!ProsesBlockAdd(l, 5, &tipe, &posisi, &teks), "add posisi huruf");
    posisi = BuatKata("2.5");
    cek(!ProsesBlockAdd(l, 5, &tipe, &posisi, &teks), "add posisi desimal");
    posisi = BuatKata("");
    cek(!ProsesBlockAdd(l, 5, &tipe, &posisi, &teks), "add posisi kosong");
    posisi = BuatKata("0");
    cek(!ProsesBlockAdd(l, 5, &tipe, &posisi, &teks), "add posisi 0");
    posisi = BuatKata("5");
    cek(!ProsesBlockAdd(l, 5, &tipe, &posisi, &teks), "add posisi melewati n+1");

    posisi = BuatKata("1");
    teks = BuatKata("");
    cek(!ProsesBlockAdd(l, 5, &tipe, &posisi, &teks), "add teks kosong");
    cek(JumlahBlok(l) == 3 && urutanSama(l, "4,2,3"), "add gagal tidak mengubah list");
}

static void ujiPerintah(void) {
    ListBlok l;
    Kata teks, posisi, status;

    printf("contoh output\n");
    BuatListBlok(&l);

    ujiPerintahAdd(&l);
    ujiPerintahAddGagal(&l);

    teks = BuatKata("Portal ditemukan dekat gerbang timur.");
    cek(ProsesBlockEdit(&l, 4, &teks) && teksSama(&l, 4, "Portal ditemukan dekat gerbang timur."), "perintah edit");
    cek(!ProsesBlockEdit(&l, 99, &teks), "edit ID tidak ada");
    teks = BuatKata("");
    cek(!ProsesBlockEdit(&l, 4, &teks), "edit teks kosong");

    posisi = BuatKata("3");
    cek(ProsesBlockMove(&l, 3, 4, &posisi) && urutanSama(&l, "2,3,4"), "perintah move");
    posisi = BuatKata("x");
    cek(!ProsesBlockMove(&l, 3, 4, &posisi), "move posisi huruf");
    posisi = BuatKata("9");
    cek(!ProsesBlockMove(&l, 3, 4, &posisi), "move posisi di luar rentang");
    posisi = BuatKata("1");
    cek(!ProsesBlockMove(&l, 3, 99, &posisi), "move ID tidak ada");
    cek(urutanSama(&l, "2,3,4"), "move gagal tidak mengubah list");

    status = BuatKata("");
    cek(!ProsesCheck(&l, 2, &status), "check status kosong ditolak");
    status = BuatKata("DONE");
    cek(ProsesCheck(&l, 2, &status) && CariBlok(&l, 2)->selesai, "check DONE");
    status = BuatKata("TODO");
    cek(ProsesCheck(&l, 2, &status) && !CariBlok(&l, 2)->selesai, "check TODO");
    status = BuatKata("SELESAI");
    cek(!ProsesCheck(&l, 2, &status), "check status tidak valid");
    status = BuatKata("DONE");
    cek(!ProsesCheck(&l, 4, &status), "check pada blok TEXT");
    cek(!ProsesCheck(&l, 99, &status), "check ID tidak ada");

    cek(ProsesBlockDelete(&l, 3) && urutanSama(&l, "2,4"), "perintah delete");
    cek(!ProsesBlockDelete(&l, 3), "delete ID tidak ada");

    printf("daftar blok\n");
    CetakDaftarBlok(&l);
    HapusSemuaBlok(&l);
    CetakDaftarBlok(&l);
    printf("selesai\n");
}

int main(void) {
    ujiTambah();
    ujiUbah();
    ujiHitung();
    ujiPindah();
    ujiHapus();
    ujiTipe();
    ujiPerintah();

    printf("Block driver: %d/%d lulus\n", total - gagal, total);
    return gagal == 0 ? 0 : 1;
}
