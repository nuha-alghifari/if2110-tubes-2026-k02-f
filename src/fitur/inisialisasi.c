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
            KosongkanJurnal(&baru);  // sekarang isinya jurnal lama (kosong)
            J->perubahan = false;
            printf("Konfigurasi berhasil dimuat. %s\n", pesan);
            return true;
        }
        KosongkanJurnal(&baru);
        printf("Pemuatan gagal: %s\n", pesan);
    }
}