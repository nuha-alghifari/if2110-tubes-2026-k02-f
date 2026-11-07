#include <stdio.h>
#include "kata.h"

static int total = 0;
static int gagal = 0;

static void cek(bool kondisi, const char *nama){
    total++;
    if (!kondisi){
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

static bool keInt(const char *s, int *hasil){
    Kata k;
    KataDariC(&k, s);
    return KataKeInt(&k, hasil);
}

static bool keID(const char *s, char prefix, int *hasil){
    Kata k;
    KataDariC(&k, s);
    return KataKeID(&k, prefix, hasil);
}

int main (void){
    Kata a, b;
    int n = 0;
    int i;

    KataDariC(&a, " haloo \t");
    PangkasKata(&a);
    cek (KataSamaC(&a, "haloo") && a.length == 5, "pangkas spasi");
    KataDariC(&a, "   ");
    PangkasKata(&a);
    cek (a.length == 0, "pangkas semua spasi");

    KosongKata(&a);
    for (i = 0; i < Max_Kata; i++){
        TambahKata(&a, 'a');
    }
    cek (a.length == Max_Kata, "Isi sampe penuh");
    cek (!TambahKata(&a, 'y'), "tambah pas penuh ditolak");

    KataDariC(&a, "Haloo");
    KataDariC(&b, "haloo");
    cek (!KataSama(&a, &b), "beda kapital");
    cek (BandingKataTanpaKapital(&a, &b) == 0, "sama tanpa kapital");
    KataDariC(&b, "Hello World");
    cek (BandingKataTanpaKapital(&a, &b) < 0, "lebih pendek");
    cek (BandingKataTanpaKapital(&b, &a) > 0, "lebih panjang");

    KataDariC(&a, "Hello World");
    KataDariC(&b, "WORLD");
    cek(KataMengandung(&a, &b), "mengandung");
    KataDariC(&a, "Dunia");
    cek(!KataMengandung(&a, &b), "tidak mengandung");
    KataDariC(&b, "Hello World Cup");
    cek(!KataMengandung(&a, &b), "lebih panjang dari teks");

    cek(keInt("12", &n) && n == 12, " int 12");
    cek(keInt("-3", &n) && n == -3, "int -3");
    cek(keInt("0", &n) && n == 0, "int 0");
    cek(!keInt("", &n), "int kosong ditolak");
    cek(!keInt("-", &n), "int minus saja ditolak");
    cek(!keInt("abc", &n), "int huruf ditolak");
    cek(!keInt("1.5", &n), "int desimal ditolak");
    cek(!keInt("1 2", &n), "int berspasi ditolak");
    cek(!keInt("12345678901", &n), "int terlalu panjang ditolak");

    cek(keID("P1", 'P', &n) && n == 1, "ID P1");
    cek(keID("P0", 'P', &n) && n == 0, "ID P0");
    cek(keID("B25", 'B', &n) && n == 25, "ID B25");
    cek(!keID("P", 'P', &n), "ID tanpa angka ditolak");
    cek(!keID("p1", 'P', &n), "ID huruf kecil ditolak");
    cek(!keID("B1", 'P', &n), "ID prefix salah ditolak");
    cek(!keID("P1a", 'P', &n), "ID ada huruf ditolak");
    cek(!keID("P-1", 'P', &n), "ID negatif ditolak");

    printf("Kata driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}
