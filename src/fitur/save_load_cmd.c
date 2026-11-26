#include <stdio.h>
#include <sys/stat.h>
#include "save_load_cmd.h"
#include "input.h"

void CMDLoad(Jurnal *J) {
    Kata folder;
    Jurnal jurnalSementara;
    char pesan[Max_Pesan];
    bool ganti = false;
    
    if (Tanya("Masukkan folder konfigurasi:", &folder) != InputOK || folder.length == 0) {
        return;
    }

    InitJurnal(&jurnalSementara);

    if (!MuatJurnal(&jurnalSementara, folder.tab, pesan, Max_Pesan)) {
        printf("Pemuatan gagal: %s\n", pesan);
        printf("Jurnal saat ini tetap digunakan.\n");
        HapusJurnal(&jurnalSementara);
        return;
    }

    printf("Konfigurasi berhasil dibaca: %s\n", pesan);
    if (TanyaYN("Ganti jurnal saat ini? (Y/N)", &ganti) != InputOK || !ganti) {
        printf("Jurnal saat ini tetap digunakan.\n");
        HapusJurnal(&jurnalSementara);
        return;
    }

    TukarJurnal(J, &jurnalSementara);
    HapusJurnal(&jurnalSementara);
    TandaiTersimpan(J);
    printf("Jurnal berhasil dimuat.\n");
}

void CMDSave(Jurnal *J) {
    Kata folder;
    struct stat info;
    char pesan[Max_Pesan];

    if (Tanya("Masukkan folder penyimpanan:", &folder) != InputOK || folder.length == 0) {
        return;
    }

    if (stat(folder.tab, &info) != 0 || !S_ISDIR(info.st_mode)) {
        printf("Folder belum tersedia. Membuat folder...\n");
        #ifdef _WIN32
            mkdir(folder.tab);
        #else
            mkdir(folder.tab, 0777);
        #endif
        if (stat(folder.tab, &info) != 0 || !S_ISDIR(info.st_mode)) {
            printf("Penyimpanan gagal: folder tidak dapat dibuat (pastikan parent directory sudah ada).\n");
            return;
        }
    }

    if (!SimpanJurnal(J, folder.tab, pesan, Max_Pesan)) {
        printf("Penyimpanan gagal: %s\n", pesan);
        return;
    }

    TandaiTersimpan(J);
    printf("Penyimpanan berhasil dilakukan!\n");
}