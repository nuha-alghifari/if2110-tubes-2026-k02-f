#ifndef JURNAL_H
#define JURNAL_H
#include <stdbool.h>
#define Max_Pesan 256 // panjang maksimal pesan hasil jurnal

typedef struct {
    int nextPageId; // Id berikutnya yang akan dipakai
    int nextBlockId;
    int nextTagId;
    bool perubahan; //true kalau ada perubahan yang belum diSave
} Jurnal;

void InitJurnal(Jurnal *j); //Inisialisasi Jurnal
void KosongkanJurnal(Jurnal *j); //membebaskan semua memori ADT dan InitJurnal
void TukarJurnal(Jurnal *a, Jurnal *b); //tukar isi kedua jurnal
int AmbilIDHalaman(Jurnal *j); 
int AmbilIDBlok(Jurnal *j);
int AmbilIDTag(Jurnal *j);
void TandaiUbah(Jurnal *j); //dipanggil setiap data diubah
void TandaiTersimpan(Jurnal *j); //dipanggil setelah save berhasil
bool MuatJurnal(Jurnal *tujuan, const char *folder, char *pesan, int pesanMax); //true dan pesan berisi ringkas, false dan pesan berisi alasan

#endif