#include "kata.h"

static char HurufKecil(char c){
    if (c >= 'A' && c <= 'Z'){
        return c - 'A' + 'a';
    }
    return c;
}

static bool spasi(char c){
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void KosongKata (Kata *k){
    k->tab[0] = '\0';
    k->length = 0;
}

bool TambahKata (Kata *k, char c){
    if (k->length >= Max_Kata){
        return false;
    }
    k->tab[k->length] = c;
    k->length++;
    k->tab[k->length] = '\0';
    return true;
}

void KataDariC(Kata *k, const char *c){
    int i = 0;
    KosongKata(k);
    while (c[i] != '\0' && TambahKata(k, c[i])){
        i++;
    }
}

void PangkasKata (Kata *k){
    int awal = 0;
    int akhir = k->length;
    int i;
    while (awal < akhir && spasi(k->tab[awal])) {
        awal++;
    }
    while (akhir > awal && spasi(k->tab[akhir - 1])) {
        akhir--;
    }
    for (i = 0; i < akhir - awal; i++) {
        k->tab[i] = k->tab[awal + i];
    }
    k->length = akhir - awal;
    k->tab[k->length] = '\0';
}

bool KataSama (const Kata *a, const Kata *b){
    return KataSamaC(a, b->tab);
}

bool KataSamaC(const Kata *a, const char *b){
    int i = 0;
    while (i < a->length && b[i] != '\0' && a->tab[i] == b[i]){
        i++;
    }
    return i == a->length && b[i] == '\0';
}

int BandingKataTanpaKapital(const Kata *a, const Kata *b){
    int i = 0;
    while (i < a->length && i < b-> length){
        char x = HurufKecil(a->tab[i]);
        char y = HurufKecil(b->tab[i]);
        if (x != y){
            if (x < y) return -1;
            else return 1;
        }
        i++;
    }
    if (a->length == b->length) return 0;
    if (a-> length < b->length) return -1;
    return 1;
}

bool KataMengandung(const Kata *teks, const Kata *kunci){
    int i, j;
    for (i = 0; i + kunci->length <= teks->length; i++){
        for (j = 0; j < kunci->length; j++){
            if (HurufKecil(teks->tab[i + j]) != HurufKecil(kunci->tab[j])) break;
        }
        if (j == kunci->length) return true;
    }
    return false;
}

static bool Angka (const char *a, int n, int *hasil){
    int i;
    int nilai = 0;
    if (n < 1 || n > 9) return false;
    for (i = 0; i < n; i++){
        if (a[i] < '0' || a[i] > '9') return false;
        nilai = nilai * 10 + (a[i] - '0');
    }
    *hasil = nilai;
    return true;
}

bool KataKeInt(const Kata *k, int *hasil) {
  int nilai;
  if (k->length > 0 && k->tab[0] == '-') {
    if (!Angka(k->tab + 1, k->length - 1, &nilai)) {
      return false;
    }
    *hasil = -nilai;
    return true;
  }
  if (!Angka(k->tab, k->length, &nilai)) {
    return false;
  }
  *hasil = nilai;
  return true;
}

bool KataKeID(const Kata *k, char prefix, int *nomor) {
  if (k->length < 2 || k->tab[0] != prefix) {
    return false;
  }
  return Angka(k->tab + 1, k->length - 1, nomor);
}
