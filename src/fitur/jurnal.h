#ifndef JURNAL_H
#define JURNAL_H
#include <stdbool.h>
#include "tree.h"
#include "multilistPage.h"
#include "multilistTag.h"
#include "GraphPage.h"
#include "trie.h"
#define Max_Pesan 256 // panjang maksimal pesan hasil jurnal

typedef struct {
    int nextPageId; // Id berikutnya yang akan dipakai
    int nextBlockId;
    int nextTagId;
    AddressPage root;
    MultiListPage pageTags;
    MultiListTag tagPages;
    Graph linkKeluar;
    Graph linkMasuk;
    Trie trieHalaman;
    Trie trieTag;
    bool perubahan; //true kalau ada perubahan yang belum diSave
} Jurnal;

void InitJurnal(Jurnal *j); //Inisialisasi Jurnal
void HapusJurnal(Jurnal *j); //membebaskan semua memori ADT; setelah ini j tidak boleh dipakai sebelum InitJurnal
void KosongkanJurnal(Jurnal *j); //HapusJurnal lalu InitJurnal, dipakai di command NEW
void TukarJurnal(Jurnal *a, Jurnal *b); //tukar isi kedua jurnal
int AmbilIDHalaman(Jurnal *j); 
int AmbilIDBlok(Jurnal *j);
int AmbilIDTag(Jurnal *j);
void TandaiUbah(Jurnal *j); //dipanggil setiap data diubah
void TandaiTersimpan(Jurnal *j); //dipanggil setelah save berhasil
int HitungHalaman(const Jurnal *j); //jumlah halaman, P0 tidak dihitung
int HitungBlok(const Jurnal *j); //jumlah blok di semua halaman
int HitungTag(const Jurnal *j); //jumlah tag
AddressPage CariHalamanJurnal(const Jurnal *j, int id);   //id 0 = P0, NULL jika tidak ada
AddressPage CariPemilikBlok(const Jurnal *j, int idBlok); //halaman pemilik blok, NULL jika tidak ada
bool MuatJurnal(Jurnal *tujuan, const char *folder, char *pesan, int pesanMax); //true dan pesan berisi ringkas, false dan pesan berisi alasan
bool SimpanJurnal(const Jurnal *j, const char *folder, char *pesan, int pesanMax);

#endif