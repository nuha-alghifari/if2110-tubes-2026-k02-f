#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "mesin_kata.h"

static MesinKarakter mesin;
static bool sudahStart = false; //mesin distart saat masukan pertama dibaca
static bool habis = false; //true kalau masukan udah abis

bool InputHabis(void){
    return habis;
}

static StatusInput Baca(const char *pertanyaan, Kata *hasil){
    StatusBaca s;
    if (pertanyaan != NULL){
        printf("%s", pertanyaan);
    }
    printf(">>>");
    fflush(stdout);

    if(!sudahStart){
        StartFile(&mesin, stdin);
        sudahStart = true;
    }

    s = BacaMasukan(&mesin, hasil);
    if(s == BacaEOF){
        habis = true;
        printf("\n");
        return InputEOF;
    }
    if (s == BacaTerlaluPanjang){
        printf("Masukan terlalu panjang\n");
        return InputSalah;
    }
    return InputOK;
}

StatusInput BacaPerintah(Kata *hasil){
    return Baca(NULL, hasil);
}

StatusInput Tanya (const char *pertanyaan, Kata *hasil){
    return Baca(pertanyaan, hasil);
}

StatusInput TanyaTidakKosong(const char *pertanyaan, Kata *hasil){
    StatusInput s = Baca(pertanyaan, hasil);
    if (s == InputOK && hasil->length == 0){
        printf("Masukkan tidak boleh kosong\n");
        return InputSalah;
    }
    return s;
}

StatusInput TanyaID(const char *pertanyaan, char prefix, int *nomor){
    Kata k;
    StatusInput s = Baca(pertanyaan, &k);
    if (s != InputOK) return s;
    if (!KataKeID(&k, prefix, nomor)){
        printf("ID tidak valid: gunakan format %c<angka>, contoh %c1.\n", prefix, prefix);
        return InputSalah;
    }
    return InputOK;
}

StatusInput TanyaInt(const char *pertanyaan, int *nilai){
    Kata k;
    StatusInput s = Baca(pertanyaan, &k);
    if (s != InputOK) return s;
    if (!KataKeInt(&k, nilai)){
        printf("Masukan harus berupa bilangan bulat.\n");
        return InputSalah;
    }
    return InputOK;
}

StatusInput TanyaYN(const char *pertanyaan, bool *ya){
    Kata k;
    StatusInput s = Baca(pertanyaan, &k);
    if (s != InputOK) return s;
    if (k.length == 1 && (k.tab[0] == 'Y' || k.tab[0] == 'y')){
        *ya = true;
        return InputOK;
    }
    if (k.length == 1 && (k.tab[0] == 'N' || k.tab[0] == 'n')){
        *ya = false;
        return InputOK;
    }
    printf("Jawaban harus Y atau N.\n");
    return InputSalah;
}