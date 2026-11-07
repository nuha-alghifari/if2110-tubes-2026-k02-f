#ifndef KATA_H
#define KATA_H
#include <stdbool.h>

#define Max_Kata 1023

typedef struct {
    char tab[Max_Kata + 1];
    int length;
} Kata;

void KosongKata(Kata *k);
bool TambahKata(Kata *k, char c);
void KataDariC(Kata *k, const char *c);
void PangkasKata(Kata *k);

bool KataSama(const Kata *a, const Kata *b);
bool KataSamaC(const Kata *a, const char *b);
int BandingKataTanpaKapital(const Kata *a, const Kata *b);
bool KataMengandung(const Kata *teks, const Kata *tab);

bool KataKeInt(const Kata *k, int *hasil);
bool KataKeID(const Kata *k, char prefix, int *nomor);

#endif