#include <stdio.h>
#include "inisialisasi.h"
#include "input.h"

bool Inisialisasi(Jurnal *J){
    printf("Selamat datang di NangorNotes!\n");

    while (true){
        Kata folder;
        Jurnal baru;
        char pesan[Max_Pesan];
        StatusInput s = Tanya("Masukkan folder konfigurasi:", &folder);

        if (s == InputEOF) return false;
        if (s == InputSalah) continue;
        if (folder.length == 0){
            printf("Pemuatan gagal: folder tidak boleh kosong.\n");
            continue;
        }

        // dimuat ke jurnal sementara dulu, kalo semuanya berhasil baru dituker
        InitJurnal(&baru);
        if (MuatJurnal(&baru, folder.tab, pesan, Max_Pesan)){
            TukarJurnal(J, &baru);
            HapusJurnal(&baru);  // sekarang isinya jurnal lama (kosong)
            J->perubahan = false;
            if (HitungHalaman(J) == 0 && HitungBlok(J) == 0 && HitungTag(J) == 0){
                printf("Jurnal kosong berhasil dimuat.\n");
            } else {
                printf("Konfigurasi berhasil dimuat. Jurnal berisi %d halaman, %d blok, dan %d tag.\n", HitungHalaman(J), HitungBlok(J), HitungTag(J));
            }
            return true;
        }
        HapusJurnal(&baru);
        printf("Pemuatan gagal: %s\n", pesan);
    }
}