#include <stdio.h>
#include "page_cmd.h"
#include "page.h"
#include "input.h"

static Kata IdHalaman(int nomor) {
    Kata k;
    k.length = sprintf(k.tab, "P%d", nomor);
    return k;
}

void CMDPageAdd(Jurnal *J) {
    int idParent;
    Kata judul;

    if (TanyaID("Masukkan ID parent:", 'P', &idParent) != InputOK) return;
    if (CariHalamanJurnal(J, idParent) == NULL) {
        printf("Halaman tidak dapat dibuat: parent P%d tidak ditemukan.\n", idParent);
        return;
    }
    if (TanyaTidakKosong("Masukkan judul halaman:", &judul) != InputOK) return;

    PageAdd(J, IdHalaman(idParent), judul);
}

void CMDPageTitle(Jurnal *J) {
    int id;
    Kata judul, judulLama;
    AddressPage halaman;

    if (TanyaID("Masukkan ID halaman:", 'P', &id) != InputOK) return;
    if (id == 0) {
        printf("P0 (Workspace) tidak dapat diubah.\n");
        return;
    }
    halaman = CariHalamanJurnal(J, id);
    if (halaman == NULL) {
        printf("Halaman P%d tidak dapat ditemukan.\n", id);
        return;
    }
    if (TanyaTidakKosong("Masukkan judul baru:", &judul) != InputOK) return;

    judulLama = halaman->title;
    if (PageTitle(J->root, IdHalaman(id), judul)) {
        TrieHapus(&J->trieHalaman, judulLama.tab, id);
        TrieTambah(&J->trieHalaman, judul.tab, id);
        TandaiUbah(J);
    }
}

void CMDPageView(Jurnal *J) {
    int id;

    if (TanyaID("Masukkan ID halaman:", 'P', &id) != InputOK) return;
    PageView(J, IdHalaman(id));
}

void CMDTree(Jurnal *J) {
    int id;

    if (TanyaID("Masukkan ID halaman:", 'P', &id) != InputOK) return;
    Tree(J->root, IdHalaman(id));
}

void CMDPath(Jurnal *J) {
    int id;

    if (TanyaID("Masukkan ID halaman:", 'P', &id) != InputOK) return;
    Path(J->root, IdHalaman(id));
}

void CMDPageStats(Jurnal *J) {
    int id;

    if (TanyaID("Masukkan ID halaman:", 'P', &id) != InputOK) return;
    PageStats(J->root, IdHalaman(id));
}

void CMDPageMove(Jurnal *J) {
    int id;
    int idParent;

    if (TanyaID("Masukkan ID halaman:", 'P', &id) != InputOK) return;
    if (id == 0) {
        printf("Root tidak bisa dipindah.\n");
        return;
    }
    if (CariHalamanJurnal(J, id) == NULL) {
        printf("Halaman P%d tidak dapat ditemukan.\n", id);
        return;
    }
    if (TanyaID("Masukkan ID parent baru:", 'P', &idParent) != InputOK) return;

    if (PageMove(J->root, IdHalaman(id), IdHalaman(idParent))) {
        TandaiUbah(J);
    }
}

void CMDPageDelete(Jurnal *J) {
    int id;
    bool ya;
    char pertanyaan[128];
    AddressPage halaman;

    if (TanyaID("Masukkan ID halaman:", 'P', &id) != InputOK) return;
    if (id == 0) {
        printf("Root tidak boleh dihapus.\n");
        return;
    }
    halaman = CariHalamanJurnal(J, id);
    if (halaman == NULL) {
        printf("Halaman P%d tidak dapat ditemukan.\n", id);
        return;
    }

    snprintf(pertanyaan, sizeof(pertanyaan), "Hapus P%d dan subhalamannya: %d halaman, %d blok? (Y/N)",
             id, countPages(halaman), countBlocks(halaman));
    if (TanyaYN(pertanyaan, &ya) != InputOK) return;
    if (!ya) {
        printf("Penghapusan dibatalkan.\n");
        return;
    }

    PageDelete(J, IdHalaman(id));
}