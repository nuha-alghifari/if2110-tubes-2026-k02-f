#include <stdio.h>
#include "input.h"

int main(void){
    Kata k;
    int n;
    bool ya;

    if (Tanya("Ketik teks bebas:", &k) == InputOK) printf("teks = '%s'\n", k.tab);
    if (TanyaTidakKosong("Ketik judul (tidak boleh kosong):", &k) == InputOK) printf("judul = '%s'\n", k.tab);
    if (TanyaID("Masukkan ID halaman (contoh P3):", 'P', &n) == InputOK) printf("nomor = %d\n", n);
    if (TanyaInt("Masukkan posisi (bilangan bulat):", &n) == InputOK) printf("angka = %d\n", n);
    if (TanyaYN("Lanjut? (Y/N)", &ya) == InputOK) printf("jawaban = %s\n", ya ? "ya" : "tidak");
    if (InputHabis()) printf("(masukan habis)\n");
    return 0;
}