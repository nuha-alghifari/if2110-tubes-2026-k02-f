#ifndef MESIN_CSV_H
#define MESIN_CSV_H
#include "mesin_karakter.h"
#include "kata.h"

typedef enum {
    CsvOK, // satu field terbaca
    CsvEOF, // sumber sudah abis
    CsvSalahFormat // kutip tdk ditutup, ada teks setelah kutip, ada newline, atau field terlalu panjang
} StatusCSV;

typedef enum {
    pemisahKoma, //kalo ada field lain di baris
    pemisahBaris //field terakhir
} PemisahCSV;

StatusCSV BacaFieldCSV (MesinKarakter *m, Kata *hasil, bool *berkutip, PemisahCSV *pemisah); //membaca satu field csv dari mesin
#endif