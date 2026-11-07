#ifndef MESIN_KARAKTER_H
#define MESIN_KARAKTER_H
#include <stdio.h>
#include <stdbool.h>

//ADT untuk mesin karakter yang membaca satu per satu karakter dari file atau string
typedef struct { 
    FILE *file; // NULL jika berasal dari string
    const char *str; // NULL jika berasa dari File
    int idx; // posisi baca berikutnya
    char cc; //current character
    bool eop; // true jika sudah beres
} MesinKarakter;

void StartFile(MesinKarakter *m, FILE *f); //Membaca dari File
void StartString(MesinKarakter *m, const char *s); //Membaca dari String
void Adv(MesinKarakter *m); //Maju satu karakter

#endif