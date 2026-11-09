#ifndef MESIN_KATA_H
#define MESIN_KATA_H
#include "mesin_karakter.h"
#include "kata.h"

typedef enum {
    BacaOK, //masukan terbaca
    BacaEOF, //sumber habis sebelum ketemu ";"
    BacaTerlaluPanjang //masukan melebihi Max_Kata
} StatusBaca;

StatusBaca BacaMasukan (MesinKarakter *m, Kata *hasil); //membaca karakter dari mesin sampai ";", spasi/newline dipangkas

#endif