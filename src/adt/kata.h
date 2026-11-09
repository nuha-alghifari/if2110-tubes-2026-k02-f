#ifndef KATA_H
#define KATA_H
#include <stdbool.h>

#define Max_Kata 1023 //panjang maksimal kata

typedef struct {
    char tab[Max_Kata + 1]; //isi hurufnya
    int length; //jumlah huruf
} Kata;

void KosongKata(Kata *k); //mengosongkan Kata
bool TambahKata(Kata *k, char c); //tambah satu huruf, false kalau udah penuh
void KataDariC(Kata *k, const char *c); //mengisi kata dari string
void PangkasKata(Kata *k); // buang spasi, tab, newline

bool KataSama(const Kata *a, const Kata *b); //true kalau persis sama
bool KataSamaC(const Kata *a, const char *b); //true kalau sama dengan string
int BandingKataTanpaKapital(const Kata *a, const Kata *b); //-1 kalau a duluan, 0 kalau b duluan
bool KataMengandung(const Kata *teks, const Kata *tab); //true kalau teks mengandung tab

bool KataKeInt(const Kata *k, int *hasil); //mengubah string ke int, false kalau bukan bil bulat
bool KataKeID(const Kata *k, char prefix, int *nomor); //P12 jadi 12 dengan prefix P, false kalau salah format

#endif