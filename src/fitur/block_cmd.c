#include <stdio.h>
#include "block_cmd.h"
#include "input.h"

static void CetakUrutanID(const ListBlok *list) {
    AddressBlok p;

    for (p = list->first; p != NULL; p = p->next) {
        printf("B%d%s", p->id, p->next != NULL ? ", " : "");
    }
}

void CetakDaftarBlok(const ListBlok *list) {
    AddressBlok p;
    int posisi = 1;

    if (list->first == NULL) {
        printf("(kosong)\n");
        return;
    }
    for (p = list->first; p != NULL; p = p->next) {
        if (p->tipe == BlokCheck) {
            printf("%d. [B%d] CHECK [%c] %s\n", posisi, p->id, p->selesai ? 'x' : ' ', p->teks.tab);
        } else {
            printf("%d. [B%d] TEXT %s\n", posisi, p->id, p->teks.tab);
        }
        posisi++;
    }
}

// Cetak alasan gagal 
static void CetakGagal(StatusBlok status, int idBlok, int batasPosisi) {
    switch (status) {
        case BlokTidakDitemukan:
            printf("Blok B%d tidak ditemukan.\n", idBlok);
            break;
        case BlokTeksKosong:
            printf("Teks tidak boleh kosong.\n");
            break;
        case BlokPosisiDiLuarRentang:
            printf("Posisi harus antara 1 dan %d.\n", batasPosisi);
            break;
        case BlokBukanChecklist:
            printf("Blok B%d bukan checklist.\n", idBlok);
            break;
        default:
            printf("Memori tidak cukup.\n");
            break;
    }
}

bool ProsesBlockAdd(ListBlok *list, int idBlokBaru, const Kata *tipe, const Kata *posisi, const Kata *teks) {
    TipeBlok tipeBlok;
    StatusBlok status;
    int pos;

    if (!KataKeTipeBlok(tipe, &tipeBlok)) {
        printf("Tipe harus TEXT atau CHECK.\n");
        return false;
    }
    if (!KataKeInt(posisi, &pos)) {
        printf("Posisi harus berupa bilangan bulat.\n");
        return false;
    }

    status = TambahBlok(list, idBlokBaru, tipeBlok, teks, pos);
    if (status != BlokOK) {
        CetakGagal(status, idBlokBaru, JumlahBlok(list) + 1);
        return false;
    }

    if (tipeBlok == BlokCheck) {
        printf("Blok B%d berhasil ditambahkan dengan status TODO.\n", idBlokBaru);
    } else {
        printf("Blok B%d berhasil ditambahkan.\n", idBlokBaru);
    }
    return true;
}

bool ProsesBlockEdit(ListBlok *list, int idBlok, const Kata *teksBaru) {
    StatusBlok status = UbahTeksBlok(list, idBlok, teksBaru);

    if (status != BlokOK) {
        CetakGagal(status, idBlok, 0);
        return false;
    }
    printf("Teks B%d berhasil diubah.\n", idBlok);
    return true;
}

bool ProsesBlockMove(ListBlok *list, int idHalaman, int idBlok, const Kata *posisiBaru) {
    StatusBlok status;
    int pos;

    if (!KataKeInt(posisiBaru, &pos)) {
        printf("Posisi harus berupa bilangan bulat.\n");
        return false;
    }

    status = PindahBlok(list, idBlok, pos);
    if (status != BlokOK) {
        CetakGagal(status, idBlok, JumlahBlok(list));
        return false;
    }

    printf("B%d berhasil dipindahkan. Urutan blok P%d: ", idBlok, idHalaman);
    CetakUrutanID(list);
    printf(".\n");
    return true;
}

bool ProsesBlockDelete(ListBlok *list, int idBlok) {
    StatusBlok status = HapusBlok(list, idBlok);

    if (status != BlokOK) {
        CetakGagal(status, idBlok, 0);
        return false;
    }
    printf("Blok B%d berhasil dihapus.\n", idBlok);
    return true;
}

bool ProsesCheck(ListBlok *list, int idBlok, const Kata *status) {
    StatusBlok hasil;
    bool selesai;

    if (KataSamaC(status, "DONE")) {
        selesai = true;
    } else if (KataSamaC(status, "TODO")) {
        selesai = false;
    } else {
        printf("Status harus TODO atau DONE.\n");
        return false;
    }

    hasil = UbahStatusBlok(list, idBlok, selesai);
    if (hasil != BlokOK) {
        CetakGagal(hasil, idBlok, 0);
        return false;
    }

    if (selesai) {
        printf("Checklist B%d berhasil ditandai selesai.\n", idBlok);
    } else {
        printf("Checklist B%d berhasil dikembalikan ke TODO.\n", idBlok);
    }
    return true;
}

static ListBlok *CariListBlokHalaman(Jurnal *J, int idHalaman) {
    AddressPage p = CariHalamanJurnal(J, idHalaman);
    if (p == NULL) return NULL;
    return &p->blocks;
}

static ListBlok *CariListBlokPemilik(Jurnal *J, int idBlok, int *idHalaman) {
    AddressPage p = CariPemilikBlok(J, idBlok);
    int nomor = 0;
    if (p == NULL) return NULL;
    KataKeID(&p->pageId, 'P', &nomor);
    *idHalaman = nomor;
    return &p->blocks;
}

static ListBlok *AmbilListPemilik(Jurnal *J, int idBlok, int *idHalaman) {
    ListBlok *list = CariListBlokPemilik(J, idBlok, idHalaman);

    if (list == NULL) {
        CetakGagal(BlokTidakDitemukan, idBlok, 0);
    }
    return list;
}

void CMDBlockAdd(Jurnal *J) {
    int idHalaman;
    int idBaru;
    Kata tipe, posisi, teks;
    ListBlok *list;

    if (TanyaID("Masukkan ID halaman:", 'P', &idHalaman) != InputOK) return;
    if (idHalaman == 0) {
        printf("P0 tidak dapat memiliki blok.\n");
        return;
    }
    list = CariListBlokHalaman(J, idHalaman);
    if (list == NULL) {
        printf("Halaman P%d tidak ditemukan.\n", idHalaman);
        return;
    }
    if (Tanya("Masukkan tipe blok (TEXT/CHECK):", &tipe) != InputOK) return;
    if (Tanya("Masukkan posisi:", &posisi) != InputOK) return;
    if (TanyaTidakKosong("Masukkan teks:", &teks) != InputOK) return;

    idBaru = J->nextBlockId;
    if (ProsesBlockAdd(list, idBaru, &tipe, &posisi, &teks)) {
        AmbilIDBlok(J);     // if sukses
        TandaiUbah(J);
    }
}

void CMDBlockEdit(Jurnal *J) {
    int idBlok;
    int idHalaman;
    Kata teksBaru;
    ListBlok *list;

    if (TanyaID("Masukkan ID blok:", 'B', &idBlok) != InputOK) return;
    list = AmbilListPemilik(J, idBlok, &idHalaman);
    if (list == NULL) return;
    if (TanyaTidakKosong("Masukkan teks baru:", &teksBaru) != InputOK) return;

    if (ProsesBlockEdit(list, idBlok, &teksBaru)) {
        TandaiUbah(J);
    }
}

void CMDBlockMove(Jurnal *J) {
    int idBlok;
    int idHalaman;
    Kata posisiBaru;
    ListBlok *list;

    if (TanyaID("Masukkan ID blok:", 'B', &idBlok) != InputOK) return;
    list = AmbilListPemilik(J, idBlok, &idHalaman);
    if (list == NULL) return;
    if (Tanya("Masukkan posisi baru:", &posisiBaru) != InputOK) return;

    if (ProsesBlockMove(list, idHalaman, idBlok, &posisiBaru)) {
        TandaiUbah(J);
    }
}

void CMDBlockDelete(Jurnal *J) {
    int idBlok;
    int idHalaman;
    ListBlok *list;

    if (TanyaID("Masukkan ID blok:", 'B', &idBlok) != InputOK) return;
    list = AmbilListPemilik(J, idBlok, &idHalaman);
    if (list == NULL) return;

    if (ProsesBlockDelete(list, idBlok)) {
        TandaiUbah(J);
    }
}

void CMDCheck(Jurnal *J) {
    int idBlok;
    int idHalaman;
    Kata status;
    ListBlok *list;

    if (TanyaID("Masukkan ID blok:", 'B', &idBlok) != InputOK) return;
    list = AmbilListPemilik(J, idBlok, &idHalaman);
    if (list == NULL) return;
    if (Tanya("Masukkan status (TODO/DONE):", &status) != InputOK) return;

    if (ProsesCheck(list, idBlok, &status)) {
        TandaiUbah(J);
    }
}
