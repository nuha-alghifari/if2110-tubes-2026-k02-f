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
    bool selesai;
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

void BuatListBlok(ListBlok *l);
bool ListBlokKosong(const ListBlok *l);
int JumlahBlok(const ListBlok *l);
int JumlahChecklist(const ListBlok *l);
int JumlahChecklistSelesai(const ListBlok *l);

AddressBlok CariBlok(const ListBlok *l, int id);
int PosisiBlok(const ListBlok *l, int id);

StatusBlok TambahBlok(ListBlok *l, int id, TipeBlok tipe, const Kata *teks, int posisi);
StatusBlok UbahTeksBlok(ListBlok *l, int id, const Kata *teksBaru);
StatusBlok PindahBlok(ListBlok *l, int id, int posisiBaru);
StatusBlok HapusBlok(ListBlok *l, int id);
StatusBlok UbahStatusBlok(ListBlok *l, int id, bool selesai);
void HapusSemuaBlok(ListBlok *l);

bool KataKeTipeBlok(const Kata *k, TipeBlok *tipe);

#endif
