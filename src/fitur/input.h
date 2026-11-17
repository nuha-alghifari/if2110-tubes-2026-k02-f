#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>
#include "kata.h"

typedef enum {
    InputOK,
    InputSalah,
    InputEOF
} StatusInput;

bool InputHabis(void); // true kalau masukan udah abis
StatusInput BacaPerintah(Kata *hasil);  // cetak ">>> " lalu baca satu masukan 
StatusInput Tanya(const char *pertanyaan, Kata *hasil); // teks bebas, boleh kosong
StatusInput TanyaTidakKosong(const char *pertanyaan, Kata *hasil); // teks bebas, tidak boleh kosong
StatusInput TanyaID(const char *pertanyaan, char prefix, int *nomor); // ID seperti P3, B2, T1, hasilnya angkanya saja
StatusInput TanyaInt(const char *pertanyaan, int *nilai); // bilangan bulat, rentangnya dicek handler
StatusInput TanyaYN(const char *pertanyaan, bool *ya); // konfirmasi Y atau N

#endif