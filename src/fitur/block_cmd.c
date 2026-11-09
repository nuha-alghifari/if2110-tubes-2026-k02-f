#include <stdio.h>
#include "block_cmd.h"

static void CetakUrutanID(const ListBlok *list){
    AddressBlok p = list->first;
    while (p != NULL){
        printf("B%d", p->id);
        if (p->next != NULL) printf(", ");
        p = p->next;
    }
}

void CetakDaftarBlok(const ListBlok *list){
    AddressBlok p = list->first;
    int pos = 1;

    if (p == NULL){
        printf("(kosong)\n");
        return;
    }
    while (p != NULL){
        if (p->tipe == BlokCheck){
            printf("%d. [B%d] CHECK [%c] %s\n", pos, p->id, p->selesai ? 'x' : ' ', p->teks.tab);
        } else {
            printf("%d. [B%d] TEXT %s\n", pos, p->id, p->teks.tab);
        }
        pos++;
        p = p->next;
    }
}

bool ProsesBlockAdd(ListBlok *list, int idBlokBaru, const Kata *tipe, const Kata *posisi, const Kata *teks){
    TipeBlok tipeBlok;
    int pos;
    StatusBlok status;

    if (!KataKeTipeBlok(tipe, &tipeBlok)){
        printf("Blok tidak dapat dibuat: tipe harus TEXT atau CHECK.\n");
        return false;
    }
    if (!KataKeInt(posisi, &pos)){
        printf("Blok tidak dapat dibuat: posisi harus berupa bilangan bulat.\n");
        return false;
    }

    status = TambahBlok(list, idBlokBaru, tipeBlok, teks, pos);
    switch (status){
        case BlokOK:
            if (tipeBlok == BlokCheck){
                printf("Blok B%d berhasil ditambahkan dengan status TODO.\n", idBlokBaru);
            } else {
                printf("Blok B%d berhasil ditambahkan.\n", idBlokBaru);
            }
            return true;
        case BlokPosisiDiLuarRentang:
            printf("Blok tidak dapat dibuat: posisi harus antara 1 dan %d.\n", JumlahBlok(list) + 1);
            return false;
        case BlokTeksKosong:
            printf("Blok tidak dapat dibuat: teks tidak boleh kosong.\n");
            return false;
        default:
            printf("Blok tidak dapat dibuat: memori tidak cukup.\n");
            return false;
    }
}

bool ProsesBlockEdit(ListBlok *list, int idBlok, const Kata *teksBaru){
    StatusBlok status = UbahTeksBlok(list, idBlok, teksBaru);
    switch (status){
        case BlokOK:
            printf("Teks B%d berhasil diubah.\n", idBlok);
            return true;
        case BlokTidakDitemukan:
            printf("Teks tidak dapat diubah: blok B%d tidak ditemukan.\n", idBlok);
            return false;
        case BlokTeksKosong:
            printf("Teks tidak dapat diubah: teks tidak boleh kosong.\n");
            return false;
        default:
            printf("Teks tidak dapat diubah.\n");
            return false;
    }
}

bool ProsesBlockMove(ListBlok *list, int idHalaman, int idBlok, const Kata *posisiBaru){
    int pos;
    StatusBlok status;

    if (!KataKeInt(posisiBaru, &pos)){
        printf("Blok tidak dapat dipindahkan: posisi harus berupa bilangan bulat.\n");
        return false;
    }

    status = PindahBlok(list, idBlok, pos);
    switch (status){
        case BlokOK:
            printf("B%d berhasil dipindahkan. Urutan blok P%d: ", idBlok, idHalaman);
            CetakUrutanID(list);
            printf(".\n");
            return true;
        case BlokTidakDitemukan:
            printf("Blok tidak dapat dipindahkan: blok B%d tidak ditemukan.\n", idBlok);
            return false;
        case BlokPosisiDiLuarRentang:
            printf("Blok tidak dapat dipindahkan: posisi harus antara 1 dan %d.\n", JumlahBlok(list));
            return false;
        default:
            printf("Blok tidak dapat dipindahkan.\n");
            return false;
    }
}

bool ProsesBlockDelete(ListBlok *list, int idBlok){
    StatusBlok status = HapusBlok(list, idBlok);
    if (status == BlokOK){
        printf("Blok B%d berhasil dihapus.\n", idBlok);
        return true;
    }
    printf("Blok tidak dapat dihapus: blok B%d tidak ditemukan.\n", idBlok);
    return false;
}

bool ProsesCheck(ListBlok *list, int idBlok, const Kata *status){
    bool selesai;
    StatusBlok hasil;

    if (KataSamaC(status, "DONE")){
        selesai = true;
    } else if (KataSamaC(status, "TODO")){
        selesai = false;
    } else {
        printf("Status tidak dapat diubah: status harus TODO atau DONE.\n");
        return false;
    }

    hasil = UbahStatusBlok(list, idBlok, selesai);
    switch (hasil){
        case BlokOK:
            if (selesai){
                printf("Checklist B%d berhasil ditandai selesai.\n", idBlok);
            } else {
                printf("Checklist B%d berhasil dikembalikan ke TODO.\n", idBlok);
            }
            return true;
        case BlokTidakDitemukan:
            printf("Status tidak dapat diubah: blok B%d tidak ditemukan.\n", idBlok);
            return false;
        case BlokBukanChecklist:
            printf("Status tidak dapat diubah: blok B%d bukan checklist.\n", idBlok);
            return false;
        default:
            printf("Status tidak dapat diubah.\n");
            return false;
    }
}

void PerintahBlockAdd(void){

}

void PerintahBlockEdit(void){

}

void PerintahBlockMove(void){

}

void PerintahBlockDelete(void){

}

void PerintahCheck(void){

}
