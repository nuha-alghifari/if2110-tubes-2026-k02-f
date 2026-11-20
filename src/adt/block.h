#ifndef BLOCK_H
#define BLOCK_H
#include <stdbool.h>
#include "kata.h"

typedef enum {
    BlokText,
    BlokCheck
} TipeBlok;

typedef struct Block *AddressBlok;

typedef struct Block {
    int id;
    TipeBlok tipe;
    Kata teks;
    bool selesai;           // hanya dipakai untuk BlokCheck
    AddressBlok next;
} Block;

typedef struct {
    AddressBlok first;
    int jumlah;
} ListBlok;

typedef enum {
    BlokOK,
    BlokGagalAlokasi,
    BlokTidakDitemukan,
    BlokTeksKosong,
    BlokPosisiDiLuarRentang,
    BlokBukanChecklist
} StatusBlok;

void BuatListBlok(ListBlok *l);                  // buat list kosong
bool ListBlokKosong(const ListBlok *l);
int JumlahBlok(const ListBlok *l);
int JumlahChecklist(const ListBlok *l);          // jumlah blok CHECK
int JumlahChecklistSelesai(const ListBlok *l);   // jumlah blok CHECK berstatus DONE

AddressBlok CariBlok(const ListBlok *l, int id); // NULL jika tidak ada
int PosisiBlok(const ListBlok *l, int id);       // 0 jika tidak ada

StatusBlok TambahBlok(ListBlok *l, int id, TipeBlok tipe, const Kata *teks, int posisi); // posisi 1..jumlah+1
StatusBlok UbahTeksBlok(ListBlok *l, int id, const Kata *teksBaru);
StatusBlok PindahBlok(ListBlok *l, int id, int posisiBaru);                              // posisi 1..jumlah
StatusBlok HapusBlok(ListBlok *l, int id);
StatusBlok UbahStatusBlok(ListBlok *l, int id, bool selesai);                            // hanya untuk CHECK
void HapusSemuaBlok(ListBlok *l);

bool KataKeTipeBlok(const Kata *k, TipeBlok *tipe);                                      // "TEXT" atau "CHECK"

#endif
