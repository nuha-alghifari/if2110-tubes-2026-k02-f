#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "../io/mesin_kata.h"

static MesinKarakter mesin;
static bool sudahStart = false; //mesin distart saat masukan pertama dibaca
static bool habis = false; //true kalau masukan udah abis

bool InputHabis(void){
    return habis;
}

static StatusInput Baca(const char *pertanyaan, Kata *hasil){
    StatusBaca s;
    if (habis){
        return InputEOF;
    }
    if (pertanyaan != NULL){
        printf("%s ", pertanyaan);
    }
    printf(">>> ");
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
    while (true){
        StatusInput s = Baca(pertanyaan, hasil);
        if (s == InputEOF) return s;
        if (s == InputOK && hasil->length > 0) return InputOK;
        if (s == InputOK) printf("Masukkan tidak boleh kosong.\n");
    }
}

StatusInput TanyaID(const char *pertanyaan, char prefix, int *nomor){
    while (true){
        Kata k;
        StatusInput s = Baca(pertanyaan, &k);
        if (s == InputEOF) return s;
        if (s != InputOK) continue;
        if (KataKeID(&k, prefix, nomor)) return InputOK;
        printf("ID tidak valid: gunakan format %c<angka> dengan huruf besar, contoh %c1.\n", prefix, prefix);
    }
}

StatusInput TanyaInt(const char *pertanyaan, int *nilai){
    while (true){
        Kata k;
        StatusInput s = Baca(pertanyaan, &k);
        if (s == InputEOF) return s;
        if (s != InputOK) continue;
        if (KataKeInt(&k, nilai)) return InputOK;
        printf("Masukan harus berupa bilangan bulat.\n");
    }
}

StatusInput TanyaYN(const char *pertanyaan, bool *ya){
    while (true){
        Kata k;
        StatusInput s = Baca(pertanyaan, &k);
        if (s == InputEOF) return s;
        if (s != InputOK) continue;
        if (KataSamaC(&k, "Y")){
            *ya = true;
            return InputOK;
        }
        if (KataSamaC(&k, "N")){
            *ya = false;
            return InputOK;
        }
        printf("Jawaban harus Y atau N (huruf besar).\n");
    }
}

StatusInput TanyaPilihan(const char *pertanyaan, const char *const opsi[], int jumlah, int *idx){
    while (true){
        Kata k;
        StatusInput s = Baca(pertanyaan, &k);
        int i;
        if (s == InputEOF) return s;
        if (s != InputOK) continue;
        for (i = 0; i < jumlah; i++){
            if (KataSamaC(&k, opsi[i])){
                *idx = i;
                return InputOK;
            }
        }
        printf("Pilihan harus ");
        for (i = 0; i < jumlah; i++){
            printf("%s%s", i > 0 ? " atau " : "", opsi[i]);
        }
        printf(" (huruf besar).\n");
    }
}