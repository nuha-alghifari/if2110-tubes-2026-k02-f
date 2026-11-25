#include "jurnal.h"
#include <stdio.h>
#include <stdlib.h>

static AddressPage BuatRoot(void){
    Kata id, judul;
    KataDariC(&id, "P0");
    KosongKata(&judul);
    return createPage(id, judul);
}

static void HapusMultiListPage(MultiListPage *l){
    AddressMultiListPage p = *l;
    AddressMultiListPage temp;
    while (p != NULL){
        temp = p;
        p = p->next;
        deleteAllTag(&temp->child);
        free(temp);
    }
    *l = NULL;
}

static void HapusMultiListTag(MultiListTag *l){
    AddressMultiListTag p = *l;
    AddressMultiListTag temp;
    while (p != NULL){
        temp = p;
        p = p->next;
        deleteAllPage(&temp->child);
        free(temp);
    }
    *l = NULL;
}

static void HapusGraph(Graph *g){
    int i;
    for (i = 0; i < g->length_vertex; i++){
        deleteAllPage(&g->node[i].Edge);
    }
    g->length_vertex = 0;
}

void InitJurnal(Jurnal *j){
    j->nextBlockId = 1;
    j->nextPageId = 1;
    j->nextTagId = 1;
    j->perubahan = false;
    j->root = BuatRoot();
    CreateMultiListPage(&j->pageTags);
    CreateMultiListTag(&j->tagPages);
    createGraph(&j->linkKeluar);
    createGraph(&j->linkMasuk);
}

void HapusJurnal(Jurnal *j){
    deleteSubtree(j->root);  // membebaskan P0, semua halaman, dan semua blok
    j->root = NULL;
    HapusMultiListPage(&j->pageTags);
    HapusMultiListTag(&j->tagPages);
    HapusGraph(&j->linkKeluar);
    HapusGraph(&j->linkMasuk);
}

void KosongkanJurnal(Jurnal *j){
    HapusJurnal(j);
    InitJurnal(j);
}

void TukarJurnal(Jurnal *a, Jurnal *b){
    Jurnal temp = *a;
    *a = *b;
    *b = temp;
}

int AmbilIDHalaman(Jurnal *j){
    int id = j->nextPageId;
    j->nextPageId++;
    return id;
}

int AmbilIDBlok(Jurnal *j){
    int id = j->nextBlockId;
    j->nextBlockId++;
    return id;
}

int AmbilIDTag(Jurnal *j){
    int id = j->nextTagId;
    j->nextTagId++;
    return id;
}

void TandaiUbah(Jurnal *j){
    j->perubahan = true;
}

void TandaiTersimpan(Jurnal *j){
    j->perubahan = false;
}

AddressPage CariHalamanJurnal(const Jurnal *j, int id){
    char buf[16];
    Kata k;
    if (id < 0) return NULL;
    snprintf(buf, sizeof(buf), "P%d", id);
    KataDariC(&k, buf);
    return findPage(j->root, k);
}

static AddressPage CariPemilikRek(AddressPage node, int idBlok){
    AddressPage hasil;
    if (node == NULL) return NULL;
    if (CariBlok(&node->blocks, idBlok) != NULL) return node;
    hasil = CariPemilikRek(node->firstChild, idBlok);
    if (hasil != NULL) return hasil;
    return CariPemilikRek(node->nextSibling, idBlok);
}

AddressPage CariPemilikBlok(const Jurnal *j, int idBlok){
    return CariPemilikRek(j->root, idBlok);
}

int HitungHalaman(const Jurnal *j){
    if (j->root == NULL) return 0;
    return countPages(j->root) - 1;
}

int HitungBlok(const Jurnal *j){
    return countBlocks(j->root);
}

int HitungTag(const Jurnal *j){
    int n = 0;
    AddressMultiListTag p = j->tagPages;
    while (p != NULL){
        n++;
        p = p->next;
    }
    return n;
}