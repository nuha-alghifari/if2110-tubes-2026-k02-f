#include <stdio.h>
#include <sys/stat.h>
#include "jurnal.h"
#include "../io/mesin_csv.h"
#include "../adt/kata.h"

//lewatin baris pertama CSV (judul kolom)
static StatusCSV LewatiHeader(MesinKarakter *m) {
    Kata k; 
    bool kutip; 
    PemisahCSV pemisah; 
    StatusCSV st;
    
    if (m->eop) return CsvEOF;
    do {
        st = BacaFieldCSV(m, &k, &kutip, &pemisah);
        if (st != CsvOK) return st;
    } while (pemisah != pemisahBaris);
    return CsvOK;
}

bool MuatJurnal(Jurnal *tujuan, const char *folder, char *pesan, int pesanMax){
    struct stat info;
    
    //cek apakah ada foldernya
    if (stat(folder, &info) != 0 || !S_ISDIR(info.st_mode)){
        snprintf(pesan, pesanMax, "folder tidak ditemukan.");
        return false;
    }

    char filepath[256];
    FILE *f;
    MesinKarakter m;
    Kata k; 
    bool kutip; 
    PemisahCSV pemisah; 
    StatusCSV st;

    snprintf(filepath, sizeof(filepath), "%s/workspace.csv", folder);
    f = fopen(filepath, "r");
    if (!f) {
        snprintf(pesan, pesanMax, "file workspace.csv tidak ditemukan.");
        return false;
    }
    
    StartFile(&m, f);
    if (LewatiHeader(&m) != CsvOK) {
        fclose(f); 
        snprintf(pesan, pesanMax, "format workspace.csv salah."); 
        return false;
    }

    int schema, nPage, nBlock, nTag;
    
    BacaFieldCSV(&m, &k, &kutip, &pemisah); 
    KataKeInt(&k, &schema);
    BacaFieldCSV(&m, &k, &kutip, &pemisah); 
    KataKeInt(&k, &nPage);
    BacaFieldCSV(&m, &k, &kutip, &pemisah); 
    KataKeInt(&k, &nBlock);
    st = BacaFieldCSV(&m, &k, &kutip, &pemisah); 
    KataKeInt(&k, &nTag);
    
    if (st == CsvSalahFormat) {
        fclose(f); 
        snprintf(pesan, pesanMax, "format workspace.csv rusak."); 
        return false;
    }

    //simpan nilai ID ke struct jurnal
    tujuan->nextPageId = nPage;
    tujuan->nextBlockId = nBlock;
    tujuan->nextTagId = nTag;
    fclose(f);

    snprintf(filepath, sizeof(filepath), "%s/pages.csv", folder);
    f = fopen(filepath, "r");
    if (!f) {
        snprintf(pesan, pesanMax, "file pages.csv tidak ditemukan.");
        return false;
    }

    StartFile(&m, f);
    LewatiHeader(&m);
    
    int totalPages = 0;
    
    while ((st = BacaFieldCSV(&m, &k, &kutip, &pemisah)) == CsvOK) {
        Kata page_id = k;
        BacaFieldCSV(&m, &k, &kutip, &pemisah);
        Kata parent_id = k;
        BacaFieldCSV(&m, &k, &kutip, &pemisah);
        
        //cek apa ada parent_id valid
        if (!KataSamaC(&parent_id, "P0")) {
            bool parentDitemukan = false; //nunggu yg lain
            if (!parentDitemukan) {
                fclose(f);
                snprintf(pesan, pesanMax, "parent %s pada pages.csv tidak ditemukan.", parent_id.tab);
                return false; 
            }
        }
        totalPages++;
    }
    
    //cek kutip
    if (st == CsvSalahFormat) {
        fclose(f);
        snprintf(pesan, pesanMax, "pages.csv memiliki format yang salah.");
        return false;
    }
    fclose(f);

    snprintf(pesan, pesanMax, "%d halaman dimuat.", totalPages);
    return true;
}