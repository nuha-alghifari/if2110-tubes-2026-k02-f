#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "jurnal.h"
#include "tag.h"
#include "../io/mesin_csv.h"
#include "../adt/kata.h"

#define BATAS_COUNTER 1000000 // batas wajar next_*_id supaya tabel bantu tidak terlalu besar

typedef struct {
    FILE *f;
    MesinKarakter m;
    int baris; // jumlah baris data yang sudah terbaca (header tidak dihitung)
} BacaCSV;

typedef enum {
    BarisOK,
    BarisHabis,
    BarisRusak
} StatusBaris;

// Tabel bantu selama pemuatan, dibebaskan di akhir MuatJurnal
typedef struct {
    AddressPage *byId;      // halaman menurut nomor ID, NULL kalau belum ada
    int *induk;             // nomor parent tiap halaman (0 = P0)
    int *urutan;            // nomor halaman menurut urutan di pages.csv
    unsigned char *adaBlok; // blok menurut nomor ID sudah dipakai atau belum
    int *posBlok;           // posisi blok menurut yang tertulis di blocks.csv
    int nPage, nBlok, nTag; // counter dari workspace.csv
    int jumlahPage, jumlahBlok, jumlahTag;
    bool terpasang;         // true kalau semua halaman sudah terpasang ke tree
} Mem;

static void Tutup(BacaCSV *b){
    if (b->f != NULL){
        fclose(b->f);
        b->f = NULL;
    }
}

// membuka file dan memeriksa baris judul kolom
static bool BukaCSV(BacaCSV *b, const char *folder, const char *nama, const char *const kolom[], int n, char *pesan, int pesanMax){
    char path[512];
    Kata k;
    bool kutip;
    PemisahCSV pem;
    StatusCSV st;
    int i;

    snprintf(path, sizeof(path), "%s/%s", folder, nama);
    b->f = fopen(path, "r");
    if (b->f == NULL){
        snprintf(pesan, pesanMax, "file %s tidak ditemukan.", nama);
        return false;
    }
    StartFile(&b->m, b->f);
    b->baris = 0;
    for (i = 0; i < n; i++){
        st = BacaFieldCSV(&b->m, &k, &kutip, &pem);
        if (st != CsvOK || kutip || !KataSamaC(&k, kolom[i]) ||
            (i < n - 1 && pem != pemisahKoma) || (i == n - 1 && pem != pemisahBaris)){
            Tutup(b);
            snprintf(pesan, pesanMax, "baris judul kolom %s tidak sesuai format.", nama);
            return false;
        }
    }
    return true;
}

// membaca satu baris data berisi tepat n field; baris kosong dilewati
static StatusBaris BacaBaris(BacaCSV *b, int n, Kata isi[], bool kutip[]){
    int i;
    PemisahCSV pem;
    StatusCSV st;
    bool ulang;

    do {
        ulang = false;
        for (i = 0; i < n && !ulang; i++){
            st = BacaFieldCSV(&b->m, &isi[i], &kutip[i], &pem);
            if (st == CsvEOF){
                return (i == 0) ? BarisHabis : BarisRusak;
            }
            if (st == CsvSalahFormat) return BarisRusak;
            if (i == 0 && isi[0].length == 0 && !kutip[0] && pem == pemisahBaris){
                ulang = true; // baris kosong
            } else if (i < n - 1 && pem != pemisahKoma){
                return BarisRusak; // field kurang
            } else if (i == n - 1 && pem != pemisahBaris){
                return BarisRusak; // field berlebih
            }
        }
    } while (ulang);
    b->baris++;
    return BarisOK;
}

static void PesanRusak(char *pesan, int max, const char *nama, const BacaCSV *b){
    snprintf(pesan, max, "format %s rusak pada baris %d.", nama, b->baris + 2);
}

// ID dengan awalan prefix dan nomor >= 1, misalnya P3, B2, T1
static bool IDValid(const Kata *k, char prefix, int *nomor){
    return KataKeID(k, prefix, nomor) && *nomor >= 1;
}

static bool NamaTagValid(const Kata *k){
    int i;
    char c;
    if (k->length == 0) return false;
    for (i = 0; i < k->length; i++){
        c = k->tab[i];
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-')) return false;
    }
    return true;
}

static void BersihkanMem(Mem *mem){
    int i;
    if (!mem->terpasang && mem->byId != NULL){
        for (i = 0; i < mem->nPage; i++){
            if (mem->byId[i] != NULL) deallocatePage(mem->byId[i]);
        }
    }
    free(mem->byId);
    free(mem->induk);
    free(mem->urutan);
    free(mem->adaBlok);
    free(mem->posBlok);
}

static bool MuatWorkspace(Mem *mem, const char *folder, char *pesan, int pesanMax){
    static const char *const kolom[] = {"schema_version", "next_page_id", "next_block_id", "next_tag_id"};
    BacaCSV b;
    Kata isi[4];
    bool kutip[4];
    int schema, np, nb, nt;
    bool ok = false;

    if (!BukaCSV(&b, folder, "workspace.csv", kolom, 4, pesan, pesanMax)) return false;
    if (BacaBaris(&b, 4, isi, kutip) != BarisOK){
        snprintf(pesan, pesanMax, "workspace.csv harus berisi satu baris data.");
    } else if (!KataKeInt(&isi[0], &schema) || !KataKeInt(&isi[1], &np) ||
               !KataKeInt(&isi[2], &nb) || !KataKeInt(&isi[3], &nt)){
        snprintf(pesan, pesanMax, "nilai pada workspace.csv harus berupa bilangan bulat.");
    } else if (BacaBaris(&b, 4, isi, kutip) != BarisHabis){
        snprintf(pesan, pesanMax, "workspace.csv harus berisi tepat satu baris data.");
    } else if (schema != 1){
        snprintf(pesan, pesanMax, "schema_version pada workspace.csv harus 1.");
    } else if (np < 1 || nb < 1 || nt < 1 || np > BATAS_COUNTER || nb > BATAS_COUNTER || nt > BATAS_COUNTER){
        snprintf(pesan, pesanMax, "counter ID pada workspace.csv tidak valid.");
    } else {
        mem->nPage = np;
        mem->nBlok = nb;
        mem->nTag = nt;
        ok = true;
    }
    Tutup(&b);
    return ok;
}

static bool MuatPages(Jurnal *j, Mem *mem, const char *folder, char *pesan, int pesanMax){
    static const char *const kolom[] = {"page_id", "parent_id", "title"};
    BacaCSV b;
    Kata isi[3];
    bool kutip[3];
    StatusBaris sb;
    bool ok = false;
    int i, id, ind, langkah;
    char buf[24];
    Kata kid;
    AddressPage node, parentNode, akhir;

    if (!BukaCSV(&b, folder, "pages.csv", kolom, 3, pesan, pesanMax)) return false;

    while ((sb = BacaBaris(&b, 3, isi, kutip)) == BarisOK){
        if (!IDValid(&isi[0], 'P', &id)){
            snprintf(pesan, pesanMax, "page_id %s pada pages.csv tidak valid (baris %d).", isi[0].tab, b.baris + 1);
            goto selesai;
        }
        if (id >= mem->nPage){
            snprintf(pesan, pesanMax, "page_id P%d melebihi next_page_id pada workspace.csv.", id);
            goto selesai;
        }
        if (mem->byId[id] != NULL){
            snprintf(pesan, pesanMax, "page_id P%d pada pages.csv muncul lebih dari sekali.", id);
            goto selesai;
        }
        if (KataSamaC(&isi[1], "P0")){
            ind = 0;
        } else if (!IDValid(&isi[1], 'P', &ind)){
            snprintf(pesan, pesanMax, "parent %s pada pages.csv tidak valid (baris %d).", isi[1].tab, b.baris + 1);
            goto selesai;
        }
        if (!kutip[2] || isi[2].length == 0){
            snprintf(pesan, pesanMax, "title P%d pada pages.csv harus diapit tanda petik dan tidak kosong.", id);
            goto selesai;
        }
        snprintf(buf, sizeof(buf), "P%d", id);
        KataDariC(&kid, buf);
        node = createPage(kid, isi[2]);
        if (node == NULL){
            snprintf(pesan, pesanMax, "memori tidak cukup.");
            goto selesai;
        }
        mem->byId[id] = node;
        mem->induk[id] = ind;
        mem->urutan[mem->jumlahPage] = id;
        mem->jumlahPage++;
    }
    if (sb == BarisRusak){
        PesanRusak(pesan, pesanMax, "pages.csv", &b);
        goto selesai;
    }

    for (i = 0; i < mem->jumlahPage; i++){
        ind = mem->induk[mem->urutan[i]];
        if (ind != 0 && (ind >= mem->nPage || mem->byId[ind] == NULL)){
            snprintf(pesan, pesanMax, "parent P%d pada pages.csv tidak ditemukan.", ind);
            goto selesai;
        }
    }

    for (i = 0; i < mem->jumlahPage; i++){
        ind = mem->urutan[i];
        langkah = 0;
        while (ind != 0){
            ind = mem->induk[ind];
            langkah++;
            if (langkah > mem->jumlahPage){
                snprintf(pesan, pesanMax, "pages.csv membentuk siklus parent.");
                goto selesai;
            }
        }
    }

    for (id = 1; id < mem->nPage; id++){
        node = mem->byId[id];
        if (node == NULL) continue;
        ind = mem->induk[id];
        parentNode = (ind == 0) ? j->root : mem->byId[ind];
        node->parent = parentNode;
        node->nextSibling = NULL;
        if (parentNode->firstChild == NULL){
            parentNode->firstChild = node;
        } else {
            akhir = parentNode->firstChild;
            while (akhir->nextSibling != NULL) akhir = akhir->nextSibling;
            akhir->nextSibling = node;
        }
    }
    mem->terpasang = true;

    for (i = 0; i < mem->jumlahPage; i++){
        id = mem->urutan[i];
        insertVertex(&j->linkKeluar, id);
        insertVertex(&j->linkMasuk, id);
        insertLastMLPage(&j->pageTags, id);
    }
    ok = true;

selesai:
    Tutup(&b);
    return ok;
}

static bool MuatBlok(Mem *mem, const char *folder, char *pesan, int pesanMax){
    static const char *const kolom[] = {"block_id", "page_id", "position", "type", "text", "checked"};
    BacaCSV b;
    Kata isi[6];
    bool kutip[6];
    StatusBaris sb;
    bool ok = false;
    int i, p, bid, pid, pos, cek, k;
    TipeBlok tipe;
    ListBlok *l;
    AddressBlok cari;

    if (!BukaCSV(&b, folder, "blocks.csv", kolom, 6, pesan, pesanMax)) return false;

    while ((sb = BacaBaris(&b, 6, isi, kutip)) == BarisOK){
        if (!IDValid(&isi[0], 'B', &bid)){
            snprintf(pesan, pesanMax, "block_id %s pada blocks.csv tidak valid (baris %d).", isi[0].tab, b.baris + 1);
            goto selesai;
        }
        if (bid >= mem->nBlok){
            snprintf(pesan, pesanMax, "block_id B%d melebihi next_block_id pada workspace.csv.", bid);
            goto selesai;
        }
        if (mem->adaBlok[bid]){
            snprintf(pesan, pesanMax, "block_id B%d pada blocks.csv muncul lebih dari sekali.", bid);
            goto selesai;
        }
        if (!IDValid(&isi[1], 'P', &pid) || pid >= mem->nPage || mem->byId[pid] == NULL){
            snprintf(pesan, pesanMax, "page_id %s pemilik blok B%d tidak ditemukan.", isi[1].tab, bid);
            goto selesai;
        }
        if (!KataKeInt(&isi[2], &pos) || pos < 1){
            snprintf(pesan, pesanMax, "position blok B%d harus bilangan bulat mulai dari 1.", bid);
            goto selesai;
        }
        if (!KataKeTipeBlok(&isi[3], &tipe)){
            snprintf(pesan, pesanMax, "type blok B%d harus TEXT atau CHECK.", bid);
            goto selesai;
        }
        if (!kutip[4] || isi[4].length == 0){
            snprintf(pesan, pesanMax, "text blok B%d harus diapit tanda petik dan tidak kosong.", bid);
            goto selesai;
        }
        if (!KataKeInt(&isi[5], &cek) || (cek != 0 && cek != 1) || (tipe == BlokText && cek == 1)){
            snprintf(pesan, pesanMax, "checked blok B%d tidak valid.", bid);
            goto selesai;
        }
        l = &mem->byId[pid]->blocks;
        if (TambahBlok(l, bid, tipe, &isi[4], JumlahBlok(l) + 1) != BlokOK){
            snprintf(pesan, pesanMax, "blok B%d gagal ditambahkan.", bid);
            goto selesai;
        }
        if (tipe == BlokCheck && cek == 1) UbahStatusBlok(l, bid, true);
        mem->adaBlok[bid] = 1;
        mem->posBlok[bid] = pos;
        mem->jumlahBlok++;
    }
    if (sb == BarisRusak){
        PesanRusak(pesan, pesanMax, "blocks.csv", &b);
        goto selesai;
    }

    for (i = 0; i < mem->jumlahPage; i++){
        pid = mem->urutan[i];
        l = &mem->byId[pid]->blocks;
        k = JumlahBlok(l);
        for (p = 1; p <= k; p++){
            cari = l->first;
            while (cari != NULL && mem->posBlok[cari->id] != p) cari = cari->next;
            if (cari == NULL){
                snprintf(pesan, pesanMax, "posisi blok pada P%d tidak berurutan dari 1.", pid);
                goto selesai;
            }
            PindahBlok(l, cari->id, p);
        }
    }
    ok = true;

selesai:
    Tutup(&b);
    return ok;
}

static bool MuatLink(Jurnal *j, Mem *mem, const char *folder, char *pesan, int pesanMax){
    static const char *const kolom[] = {"source_id", "target_id"};
    BacaCSV b;
    Kata isi[2];
    bool kutip[2];
    StatusBaris sb;
    bool ok = false;
    int s, t, idx;

    if (!BukaCSV(&b, folder, "links.csv", kolom, 2, pesan, pesanMax)) return false;

    while ((sb = BacaBaris(&b, 2, isi, kutip)) == BarisOK){
        if (!IDValid(&isi[0], 'P', &s) || s >= mem->nPage || mem->byId[s] == NULL){
            snprintf(pesan, pesanMax, "source_id %s pada links.csv tidak ditemukan.", isi[0].tab);
            goto selesai;
        }
        if (!IDValid(&isi[1], 'P', &t) || t >= mem->nPage || mem->byId[t] == NULL){
            snprintf(pesan, pesanMax, "target_id %s pada links.csv tidak ditemukan.", isi[1].tab);
            goto selesai;
        }
        if (s == t){
            snprintf(pesan, pesanMax, "halaman P%d tidak dapat menautkan dirinya sendiri.", s);
            goto selesai;
        }
        idx = SearchVertexIdx(j->linkKeluar, s);
        if (idx == INVALID_IDX || searchPage(j->linkKeluar.node[idx].Edge, t) != NULL){
            snprintf(pesan, pesanMax, "link P%d ke P%d pada links.csv muncul lebih dari sekali.", s, t);
            goto selesai;
        }
        addEdge(&j->linkKeluar, s, t);
        addEdge(&j->linkMasuk, t, s);
    }
    if (sb == BarisRusak){
        PesanRusak(pesan, pesanMax, "links.csv", &b);
        goto selesai;
    }
    ok = true;

selesai:
    Tutup(&b);
    return ok;
}

static bool MuatTag(Jurnal *j, Mem *mem, const char *folder, char *pesan, int pesanMax){
    static const char *const kolom[] = {"tag_id", "name"};
    BacaCSV b;
    Kata isi[2];
    bool kutip[2];
    StatusBaris sb;
    bool ok = false;
    int id, i;
    Tag t;
    AddressMultiListTag p;

    if (!BukaCSV(&b, folder, "tags.csv", kolom, 2, pesan, pesanMax)) return false;

    while ((sb = BacaBaris(&b, 2, isi, kutip)) == BarisOK){
        if (!IDValid(&isi[0], 'T', &id)){
            snprintf(pesan, pesanMax, "tag_id %s pada tags.csv tidak valid (baris %d).", isi[0].tab, b.baris + 1);
            goto selesai;
        }
        if (id >= mem->nTag){
            snprintf(pesan, pesanMax, "tag_id T%d melebihi next_tag_id pada workspace.csv.", id);
            goto selesai;
        }
        if (searchMLTagByID(j->tagPages, id) != NULL){
            snprintf(pesan, pesanMax, "tag_id T%d pada tags.csv muncul lebih dari sekali.", id);
            goto selesai;
        }
        if (!kutip[1] || !NamaTagValid(&isi[1])){
            snprintf(pesan, pesanMax, "name tag T%d harus diapit tanda petik dan hanya berisi huruf kecil, angka, _ atau -.", id);
            goto selesai;
        }
        for (p = j->tagPages; p != NULL; p = p->next){
            if (p->info.length == isi[1].length){
                i = 0;
                while (i < isi[1].length && p->info.kata[i] == isi[1].tab[i]) i++;
                if (i == isi[1].length){
                    snprintf(pesan, pesanMax, "nama tag \"%s\" pada tags.csv muncul lebih dari sekali.", isi[1].tab);
                    goto selesai;
                }
            }
        }
        t.id = id;
        t.length = isi[1].length;
        for (i = 0; i < isi[1].length; i++) t.kata[i] = isi[1].tab[i];
        t.kata[t.length] = '\0';
        insertLastMLTag(&j->tagPages, t);
        mem->jumlahTag++;
    }
    if (sb == BarisRusak){
        PesanRusak(pesan, pesanMax, "tags.csv", &b);
        goto selesai;
    }
    ok = true;

selesai:
    Tutup(&b);
    return ok;
}

static bool MuatPageTag(Jurnal *j, Mem *mem, const char *folder, char *pesan, int pesanMax){
    static const char *const kolom[] = {"page_id", "tag_id"};
    BacaCSV b;
    Kata isi[2];
    bool kutip[2];
    StatusBaris sb;
    bool ok = false;
    int pid, tid;
    AddressMultiListTag nodeTag;
    AddressMultiListPage nodePage;

    if (!BukaCSV(&b, folder, "page_tags.csv", kolom, 2, pesan, pesanMax)) return false;

    while ((sb = BacaBaris(&b, 2, isi, kutip)) == BarisOK){
        if (!IDValid(&isi[0], 'P', &pid) || pid >= mem->nPage || mem->byId[pid] == NULL){
            snprintf(pesan, pesanMax, "page_id %s pada page_tags.csv tidak ditemukan.", isi[0].tab);
            goto selesai;
        }
        nodeTag = NULL;
        if (IDValid(&isi[1], 'T', &tid)) nodeTag = searchMLTagByID(j->tagPages, tid);
        if (nodeTag == NULL){
            snprintf(pesan, pesanMax, "tag_id %s pada page_tags.csv tidak ditemukan.", isi[1].tab);
            goto selesai;
        }
        if (searchPage(nodeTag->child, pid) != NULL){
            snprintf(pesan, pesanMax, "pasangan P%d dan T%d pada page_tags.csv muncul lebih dari sekali.", pid, tid);
            goto selesai;
        }
        nodePage = searchMLPageByID(j->pageTags, pid);
        if (nodePage == NULL){
            snprintf(pesan, pesanMax, "halaman P%d tidak ditemukan di daftar tag.", pid);
            goto selesai;
        }
        insertSortedPage(&nodeTag->child, pid);
        insertSortedTag(&nodePage->child, nodeTag->info);
    }
    if (sb == BarisRusak){
        PesanRusak(pesan, pesanMax, "page_tags.csv", &b);
        goto selesai;
    }
    ok = true;

selesai:
    Tutup(&b);
    return ok;
}

static bool MuatIsi(Jurnal *tujuan, Mem *mem, const char *folder, char *pesan, int pesanMax){
    int i;

    if (!MuatWorkspace(mem, folder, pesan, pesanMax)) return false;

    mem->byId = (AddressPage *) malloc(sizeof(AddressPage) * mem->nPage);
    mem->induk = (int *) malloc(sizeof(int) * mem->nPage);
    mem->urutan = (int *) malloc(sizeof(int) * mem->nPage);
    mem->adaBlok = (unsigned char *) malloc(sizeof(unsigned char) * mem->nBlok);
    mem->posBlok = (int *) malloc(sizeof(int) * mem->nBlok);
    if (mem->byId == NULL || mem->induk == NULL || mem->urutan == NULL || mem->adaBlok == NULL || mem->posBlok == NULL){
        snprintf(pesan, pesanMax, "memori tidak cukup.");
        return false;
    }
    for (i = 0; i < mem->nPage; i++){
        mem->byId[i] = NULL;
        mem->induk[i] = 0;
        mem->urutan[i] = 0;
    }
    for (i = 0; i < mem->nBlok; i++){
        mem->adaBlok[i] = 0;
        mem->posBlok[i] = 0;
    }

    if (!MuatPages(tujuan, mem, folder, pesan, pesanMax)) return false;
    if (!MuatBlok(mem, folder, pesan, pesanMax)) return false;
    if (!MuatLink(tujuan, mem, folder, pesan, pesanMax)) return false;
    if (!MuatTag(tujuan, mem, folder, pesan, pesanMax)) return false;
    if (!MuatPageTag(tujuan, mem, folder, pesan, pesanMax)) return false;

    tujuan->nextPageId = mem->nPage;
    tujuan->nextBlockId = mem->nBlok;
    tujuan->nextTagId = mem->nTag;
    tujuan->perubahan = false;
    snprintf(pesan, pesanMax, "%d halaman, %d blok, %d tag.", mem->jumlahPage, mem->jumlahBlok, mem->jumlahTag);
    return true;
}

bool MuatJurnal(Jurnal *tujuan, const char *folder, char *pesan, int pesanMax){
    struct stat info;
    Mem mem;
    bool ok;

    if (stat(folder, &info) != 0 || !S_ISDIR(info.st_mode)){
        snprintf(pesan, pesanMax, "folder tidak ditemukan.");
        return false;
    }

    mem.byId = NULL;
    mem.induk = NULL;
    mem.urutan = NULL;
    mem.adaBlok = NULL;
    mem.posBlok = NULL;
    mem.nPage = mem.nBlok = mem.nTag = 0;
    mem.jumlahPage = mem.jumlahBlok = mem.jumlahTag = 0;
    mem.terpasang = false;

    ok = MuatIsi(tujuan, &mem, folder, pesan, pesanMax);
    BersihkanMem(&mem);
    return ok;
}