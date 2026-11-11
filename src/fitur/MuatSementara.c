#include <stdio.h>
#include <sys/stat.h>
#include "jurnal.h"

// cuma mengecek foldernya ada, isi file CSV belum dibaca
bool MuatJurnal(Jurnal *tujuan, const char *folder, char *pesan, int pesanMax){
    struct stat info;
    (void)tujuan;
    if (stat(folder, &info) != 0 || !S_ISDIR(info.st_mode)){
        snprintf(pesan, pesanMax, "folder tidak ditemukan.");
        return false;
    }
    snprintf(pesan, pesanMax, "(sementara: isi folder belum dibaca)");
    return true;
}