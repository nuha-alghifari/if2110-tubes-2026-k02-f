#include <stdio.h>
#include "mesin_kata.h"
#include "mesin_csv.h"
#include "mesin_karakter.h"

static int total = 0;
static int gagal = 0;

static void cek (bool kondisi, const char *nama){
    total++;
    if (!kondisi){
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

static bool masukan (MesinKarakter *m, StatusBaca st, const char *isi){
    Kata k;
    StatusBaca s = BacaMasukan (m, &k);
    return s == st && KataSamaC(&k, isi);
}

static bool field (MesinKarakter *m, const char *isi, bool kutip, PemisahCSV pemisah){
    Kata k;
    bool q;
    PemisahCSV p;
    StatusCSV s = BacaFieldCSV (m, &k, &q, &p);
    return s == CsvOK && KataSamaC(&k, isi) && q == kutip && p == pemisah;
}

static StatusCSV statusCSV(MesinKarakter *m){
    Kata k;
    bool q;
    PemisahCSV p;
    return BacaFieldCSV (m, &k, &q, &p);
}

int main (void){
    MesinKarakter m;
    static char panjang[Max_Kata + 20];
    int i;

    StartString(&m, "ab");
    cek(!m.eop && m.cc == 'a', "m karakter pertama");
    Adv(&m);
    cek(!m.eop && m.cc == 'b', "m karakter kedua");
    Adv(&m);
    cek(m.eop && m.cc == '\0', "m habis");
    Adv(&m);
    cek(m.eop && m.cc == '\0', "m adv setelah habis aman");
    StartString(&m, "");
    cek(m.eop, "m string kosong");

    StartString(&m, "  PAGEADD  ;\nHELP;\n\n  PAGEADD Rencana  Liburan ;;sisa");
    cek(masukan(&m, BacaOK, "PAGEADD"), "kata pangkas spasi");
    cek(masukan(&m, BacaOK, "HELP"), "kata newline sisa dibuang");
    cek(masukan(&m, BacaOK, "PAGEADD Rencana  Liburan"), "kata spasi dalam dipertahankan");
    cek(masukan(&m, BacaOK, ""), "kata hanya titik koma");
    cek(masukan(&m, BacaEOF, ""), "kata eof tanpa titik koma");

    StartString(&m, "");
    cek(masukan(&m, BacaEOF, ""), "kata sumber kosong");

    for (i = 0; i < Max_Kata; i++) panjang[i] = 'a';
    panjang[Max_Kata] = ';';
    panjang[Max_Kata + 1] = '\0';
    StartString(&m, panjang);
    {
        Kata k;
        cek(BacaMasukan(&m, &k) == BacaOK && k.length == Max_Kata, "kata tepat batas diterima");
    }

    for (i = 0; i < Max_Kata + 5; i++) panjang[i] = 'a';
    panjang[Max_Kata + 5] = ';';
    panjang[Max_Kata + 6] = 'H';
    panjang[Max_Kata + 7] = ';';
    panjang[Max_Kata + 8] = '\0';
    StartString(&m, panjang);
    cek(masukan(&m, BacaTerlaluPanjang, ""), "kata terlalu panjang ditolak");
    cek(masukan(&m, BacaOK, "H"), "kata sisa panjang tidak bocor");

    StartString(&m, "P1,P0,\"Catatan, \"\"Portal\"\"\"\r\nP2,P0,\"x\"\n");
    cek(field(&m, "P1", false, pemisahKoma), "csv field biasa");
    cek(field(&m, "P0", false, pemisahKoma), "csv field kedua");
    cek(field(&m, "Catatan, \"Portal\"", true, pemisahBaris), "csv koma dan kutip ganda, akhir CRLF");
    cek(field(&m, "P2", false, pemisahKoma), "csv baris kedua");
    cek(field(&m, "P0", false, pemisahKoma), "csv baris kedua field kedua");
    cek(field(&m, "x", true, pemisahBaris), "csv akhir LF");
    cek(statusCSV(&m) == CsvEOF, "csv eof setelah data habis");

    StartString(&m, "a,,b");
    cek(field(&m, "a", false, pemisahKoma), "csv tanpa newline akhir: a");
    cek(field(&m, "", false, pemisahKoma), "csv field kosong tanpa kutip");
    cek(field(&m, "b", false, pemisahBaris), "csv tanpa newline akhir: b");
    cek(statusCSV(&m) == CsvEOF, "csv eof tanpa newline akhir");

    StartString(&m, "\"\",x");
    cek(field(&m, "", true, pemisahKoma), "csv field kosong berkutip");
    cek(field(&m, "x", false, pemisahBaris), "csv setelah field kosong berkutip");

    StartString(&m, "");
    cek(statusCSV(&m) == CsvEOF, "csv sumber kosong");

    StartString(&m, "a,\"belum ditutup");
    cek(field(&m, "a", false, pemisahKoma), "csv kutip tak ditutup: a");
    cek(statusCSV(&m) == CsvSalahFormat, "csv kutip tak ditutup ditolak");

    StartString(&m, "\"x\"y,b\n");
    cek(statusCSV(&m) == CsvSalahFormat, "csv teks setelah kutip penutup ditolak");

    StartString(&m, "\"baris\nbaru\",b\n");
    cek(statusCSV(&m) == CsvSalahFormat, "csv newline di dalam kutip ditolak");

    StartString(&m, "ab\"c,d\n");
    cek(statusCSV(&m) == CsvSalahFormat, "csv kutip liar di field tanpa kutip ditolak");

    for (i = 0; i < Max_Kata + 5; i++) panjang[i] = 'a';
    panjang[Max_Kata + 5] = '\0';
    StartString(&m, panjang);
    cek(statusCSV(&m) == CsvSalahFormat, "csv field terlalu panjang ditolak");

    printf("Mesin driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}