#include <stdio.h>
#include "perintah.h"
#include "handler.h"
#include "input.h"
#include "save_load_cmd.h"
#include "page_cmd.h"
#include "block_cmd.h"
#include "pencarian.h"
#include "tagfeat.h"
#include "linkbacklink.h"

typedef struct {
    const char *nama;
    const char *deskripsi;
    Handler fungsi;
} Perintah;

static const Perintah daftar[] = {
    {"NEW", "mengosongkan jurnal", CMDNew},
    {"HELP", "menampilkan daftar perintah", CMDHelp},
    {"QUIT", "keluar dari program", CMDQuit},
    {"LOAD", "memuat jurnal dari folder", CMDLoad},
    {"SAVE", "menyimpan jurnal ke folder", CMDSave},
    {"PAGEADD", "membuat halaman baru", CMDPageAdd},
    {"PAGETITLE", "mengubah judul halaman", CMDPageTitle},
    {"PAGEVIEW", "melihat isi halaman", CMDPageView},
    {"TREE", "menampilkan hierarki halaman", CMDTree},
    {"PATH", "menampilkan lokasi halaman", CMDPath},
    {"PAGESTATS", "menampilkan ringkasan halaman", CMDPageStats},
    {"PAGEMOVE", "memindahkan halaman", CMDPageMove},
    {"PAGEDELETE", "menghapus halaman beserta subhalaman", CMDPageDelete},
    {"BLOCKADD", "menambah blok", CMDBlockAdd},
    {"BLOCKEDIT", "mengubah teks blok", CMDBlockEdit},
    {"BLOCKMOVE", "memindahkan blok", CMDBlockMove},
    {"BLOCKDELETE", "menghapus blok", CMDBlockDelete},
    {"CHECK", "mengubah status checklist", CMDCheck},
    {"SEARCH", "mencari halaman", CMDSearch},
    {"PAGES", "menampilkan halaman terurut", CMDPages},
    {"TAGADD", "membuat tag baru", CMDTagAdd},
    {"TAGDELETE", "menghapus tag dari katalog", CMDTagDelete},
    {"TAGLIST", "menampilkan katalog tag", CMDTagList},
    {"TAG", "memasang tag pada halaman", CMDTag},
    {"UNTAG", "melepas tag dari halaman", CMDUntag},
    {"PAGETAGS", "menampilkan tag suatu halaman", CMDPageTags},
    {"TAGPAGES", "menampilkan halaman pemakai tag", CMDTagPages},
    {"LINK", "menautkan halaman sumber ke tujuan", CMDLink},
    {"UNLINK", "menghapus link antar halaman", CMDUnlink},
    {"LINKS", "menampilkan halaman yang dirujuk", CMDLinks},
    {"BACKLINKS", "menampilkan halaman yang merujuk", CMDBacklinks},
    {"LINKPATH", "mencari jalur link terpendek antar halaman", CMDLinkPath},
};

#define JumlahPerintah ((int)(sizeof(daftar) / sizeof(daftar[0])))

static bool berjalan = true;  

static const Perintah *CariPerintah(const Kata *nama){
    int i;
    for (i = 0; i < JumlahPerintah; i++){
        if (KataSamaC(nama, daftar[i].nama)){
            return &daftar[i];
        }
    }
    return NULL;
}

void JalankanPerintah(Jurnal *J){
    berjalan = true;
    while (berjalan){
        Kata nama;
        const Perintah *p;
        StatusInput s = BacaPerintah(&nama);

        if (s == InputEOF) break;
        if (s == InputSalah) continue;
        if (nama.length == 0){
            printf("Perintah tidak boleh kosong, Ketik HELP untuk melihat daftar perintah yang ada.\n");
            continue;
        }

        p = CariPerintah(&nama);
        if (p == NULL){
            printf("Perintah \"%s\" tidak dikenal, Ketik HELP untuk melihat daftar perintah yang ada.\n", nama.tab);
            continue;
        }
        p->fungsi(J);
    }
}

void CMDHelp(Jurnal *J){
    int i;
    (void)J;
    printf("Daftar perintah:\n");
    for (i = 0; i < JumlahPerintah; i++){
        printf("  %-12s - %s\n", daftar[i].nama, daftar[i].deskripsi);
    }
    printf("Akhiri setiap masukan dengan tanda titik koma.\n");
}

void CMDNew(Jurnal *J){
    bool ya;
    StatusInput s;
    if (J->perubahan){
        s = TanyaYN("Ada perubahan yang belum disimpan, ingin kosongkan jurnal saat ini? (Y/N)", &ya);
    } else {
        s = TanyaYN("Kosongkan jurnal saat ini? (Y/N)", &ya);
    }

    if (s == InputEOF) return;
    if (s != InputOK || !ya){
        printf("Pembuatan jurnal baru dibatalkan.\n");
        return;
    }
    KosongkanJurnal(J);  // file yang udah pernah disimpan tidak disentuh
    printf("Jurnal baru berhasil dibuat.\n");
}

void CMDQuit(Jurnal *J){
    if (J->perubahan){
        bool ya;
        StatusInput s = TanyaYN("Ada perubahan yang belum disimpan, ingin tetap keluar? (Y/N)", &ya);
        if (s == InputEOF) return;
        if (s != InputOK || !ya){
            printf("Kembali ke NangorNotes.\n");
            return;
        }
    }
    printf("Terima kasih telah menggunakan NangorNotes!\n");
    berjalan = false;
}