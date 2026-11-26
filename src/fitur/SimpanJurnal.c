#include <stdio.h>
#include "jurnal.h"

#define MaxPath 1100

typedef struct {
    FILE *halaman;
    FILE *blok;
    int maxPage;
    int maxBlock;
    bool ok;
} Ctx;

static bool BuatPath(char *out, const char *folder, const char *nama) {
    int n = 0, i = 0;
    while (folder[i] != '\0') {
        if (n >= MaxPath - 2) return false;
        out[n++] = folder[i++];
    }
    if (n > 0 && out[n - 1] != '/' && out[n - 1] != '\\') out[n++] = '/';
    i = 0;
    while (nama[i] != '\0') {
        if (n >= MaxPath - 1) return false;
        out[n++] = nama[i++];
    }
    out[n] = '\0';
    return true;
}

static void TulisTeks(FILE *f, const char *s, int len, bool *ok) {
    int i;
    if (fputc('"', f) == EOF) *ok = false;
    for (i = 0; i < len; i++) {
        if (s[i] == '"' && fputc('"', f) == EOF) *ok = false;
        if (fputc(s[i], f) == EOF) *ok = false;
    }
    if (fputc('"', f) == EOF) *ok = false;
}

static void Telusuri(AddressPage p, AddressPage root, Ctx *c) {
    AddressPage anak;
    AddressBlok b;
    int nomor, pos;

    if (p == NULL) return;

    if (p != root) {
        if (KataKeID(&p->pageId, 'P', &nomor) && nomor > c->maxPage) c->maxPage = nomor;
        if (fprintf(c->halaman, "%s,%s,", p->pageId.tab,
                    p->parent == NULL || p->parent == root ? "P0" : p->parent->pageId.tab) < 0) {
            c->ok = false;
        }
        TulisTeks(c->halaman, p->title.tab, p->title.length, &c->ok);
        if (fputc('\n', c->halaman) == EOF) c->ok = false;

        pos = 1;
        for (b = p->blocks.first; b != NULL; b = b->next) {
            if (b->id > c->maxBlock) c->maxBlock = b->id;
            if (fprintf(c->blok, "B%d,%s,%d,%s,", b->id, p->pageId.tab, pos,
                        b->tipe == BlokCheck ? "CHECK" : "TEXT") < 0) {
                c->ok = false;
            }
            TulisTeks(c->blok, b->teks.tab, b->teks.length, &c->ok);
            if (fprintf(c->blok, ",%d\n", (b->tipe == BlokCheck && b->selesai) ? 1 : 0) < 0) {
                c->ok = false;
            }
            pos++;
        }
    }
    for (anak = p->firstChild; anak != NULL; anak = anak->nextSibling) {
        Telusuri(anak, root, c);
    }
}

static bool BukaTulis(FILE **f, const char *folder, const char *nama, char *pesan, int pesanMax) {
    char path[MaxPath];
    if (!BuatPath(path, folder, nama)) {
        snprintf(pesan, pesanMax, "path folder terlalu panjang.");
        return false;
    }
    *f = fopen(path, "w");
    if (*f == NULL) {
        snprintf(pesan, pesanMax, "tidak dapat menulis %s.", path);
        return false;
    }
    return true;
}

static bool Tutup(FILE *f) {
    return fclose(f) == 0;
}

static bool SimpanHalamanBlok(const Jurnal *j, const char *folder, int *maxP, int *maxB, char *pesan, int pesanMax) {
    Ctx c;
    bool tutupOk;

    c.maxPage = 0;
    c.maxBlock = 0;
    c.ok = true;
    c.halaman = NULL;
    c.blok = NULL;

    if (!BukaTulis(&c.halaman, folder, "pages.csv", pesan, pesanMax)) return false;
    if (!BukaTulis(&c.blok, folder, "blocks.csv", pesan, pesanMax)) {
        Tutup(c.halaman);
        return false;
    }
    if (fprintf(c.halaman, "page_id,parent_id,title\n") < 0) c.ok = false;
    if (fprintf(c.blok, "block_id,page_id,position,type,text,checked\n") < 0) c.ok = false;

    Telusuri(j->root, j->root, &c);

    tutupOk = Tutup(c.halaman);
    tutupOk = Tutup(c.blok) && tutupOk;
    if (!c.ok || !tutupOk) {
        snprintf(pesan, pesanMax, "gagal menulis pages.csv atau blocks.csv.");
        return false;
    }
    *maxP = c.maxPage;
    *maxB = c.maxBlock;
    return true;
}

static bool SimpanLink(const Jurnal *j, const char *folder, char *pesan, int pesanMax) {
    FILE *f;
    AddresslistPage e;
    bool ok = true;
    int i;

    if (!BukaTulis(&f, folder, "links.csv", pesan, pesanMax)) return false;
    if (fprintf(f, "source_id,target_id\n") < 0) ok = false;
    for (i = 0; i < j->linkKeluar.length_vertex; i++) {
        for (e = j->linkKeluar.node[i].Edge; e != NULL; e = e->next) {
            if (fprintf(f, "P%d,P%d\n", j->linkKeluar.node[i].info, e->info) < 0) ok = false;
        }
    }
    if (!Tutup(f) || !ok) {
        snprintf(pesan, pesanMax, "gagal menulis links.csv.");
        return false;
    }
    return true;
}

static bool SimpanTag(const Jurnal *j, const char *folder, int *maxT, char *pesan, int pesanMax) {
    FILE *f;
    AddressMultiListTag p;
    bool ok = true;

    *maxT = 0;
    if (!BukaTulis(&f, folder, "tags.csv", pesan, pesanMax)) return false;
    if (fprintf(f, "tag_id,name\n") < 0) ok = false;
    for (p = j->tagPages; p != NULL; p = p->next) {
        if (p->info.id > *maxT) *maxT = p->info.id;
        if (fprintf(f, "T%d,", p->info.id) < 0) ok = false;
        TulisTeks(f, p->info.kata, p->info.length, &ok);
        if (fputc('\n', f) == EOF) ok = false;
    }
    if (!Tutup(f) || !ok) {
        snprintf(pesan, pesanMax, "gagal menulis tags.csv.");
        return false;
    }
    return true;
}

static bool SimpanPageTag(const Jurnal *j, const char *folder, char *pesan, int pesanMax) {
    FILE *f;
    AddressMultiListPage p;
    AddresslistTag t;
    bool ok = true;

    if (!BukaTulis(&f, folder, "page_tags.csv", pesan, pesanMax)) return false;
    if (fprintf(f, "page_id,tag_id\n") < 0) ok = false;
    for (p = j->pageTags; p != NULL; p = p->next) {
        for (t = p->child; t != NULL; t = t->next) {
            if (fprintf(f, "P%d,T%d\n", p->info, t->info.id) < 0) ok = false;
        }
    }
    if (!Tutup(f) || !ok) {
        snprintf(pesan, pesanMax, "gagal menulis page_tags.csv.");
        return false;
    }
    return true;
}

static int Terbesar(int counter, int idMax) {
    return counter > idMax ? counter : idMax + 1;
}

static bool SimpanWorkspace(const Jurnal *j, const char *folder, int maxP, int maxB, int maxT, char *pesan, int pesanMax) {
    FILE *f;
    bool ok = true;

    if (!BukaTulis(&f, folder, "workspace.csv", pesan, pesanMax)) return false;
    if (fprintf(f, "schema_version,next_page_id,next_block_id,next_tag_id\n") < 0) ok = false;
    if (fprintf(f, "1,%d,%d,%d\n", Terbesar(j->nextPageId, maxP),
                Terbesar(j->nextBlockId, maxB), Terbesar(j->nextTagId, maxT)) < 0) ok = false;
    if (!Tutup(f) || !ok) {
        snprintf(pesan, pesanMax, "gagal menulis workspace.csv.");
        return false;
    }
    return true;
}

bool SimpanJurnal(const Jurnal *j, const char *folder, char *pesan, int pesanMax) {
    int maxP, maxB, maxT;

    if (!SimpanHalamanBlok(j, folder, &maxP, &maxB, pesan, pesanMax)) return false;
    if (!SimpanLink(j, folder, pesan, pesanMax)) return false;
    if (!SimpanTag(j, folder, &maxT, pesan, pesanMax)) return false;
    if (!SimpanPageTag(j, folder, pesan, pesanMax)) return false;
    if (!SimpanWorkspace(j, folder, maxP, maxB, maxT, pesan, pesanMax)) return false;

    snprintf(pesan, pesanMax, "%d halaman, %d blok, %d tag.", HitungHalaman(j), HitungBlok(j), HitungTag(j));
    return true;
}
