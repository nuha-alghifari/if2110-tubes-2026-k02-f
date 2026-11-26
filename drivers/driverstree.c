#include <stdio.h>
#include "tree.h"

static int total = 0;
static int gagal = 0;

static void cek(bool kondisi, const char *nama){
    total++;
    if (!kondisi){
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

static Kata kata(const char *s){
    Kata k;
    KataDariC(&k, s);
    return k;
}

static AddressPage tambah(AddressPage parent, const char *id, const char *judul){
    AddressPage p = createPage(kata(id), kata(judul));
    AddressPage terakhir;
    if (p == NULL) return NULL;
    p->parent = parent;
    if (parent->firstChild == NULL){
        parent->firstChild = p;
    } else {
        terakhir = findLastSibling(parent->firstChild);
        terakhir->nextSibling = p;
    }
    return p;
}

static void tambahBlok(AddressPage p, int id, TipeBlok tipe, const char *teks){
    Kata t = kata(teks);
    TambahBlok(&p->blocks, id, tipe, &t, JumlahBlok(&p->blocks) + 1);
}

int main(void){
    AddressPage root, p1, p2, p3, p4, p5, x;
    Kata id = kata("P1");

    x = createPage(kata("P9"), kata("Sembilan"));
    cek(x != NULL, "createPage berhasil");
    cek(KataSamaC(&x->pageId, "P9") && KataSamaC(&x->title, "Sembilan"), "createPage isi id dan judul");
    cek(x->parent == NULL && x->firstChild == NULL && x->nextSibling == NULL, "createPage pointer NULL");
    cek(JumlahBlok(&x->blocks) == 0, "createPage tanpa blok");
    deallocatePage(x);
    deallocatePage(NULL);

    root = createPage(kata("P0"), kata("Workspace"));
    cek(countPages(root) == 1, "countPages hanya akar");
    cek(getDepth(root) == 0, "kedalaman hanya akar");
    cek(findLastSibling(root) == root, "findLastSibling tanpa saudara");

    p1 = tambah(root, "P1", "Satu");
    p2 = tambah(root, "P2", "Dua");
    p3 = tambah(p1, "P3", "Tiga");
    p4 = tambah(p3, "P4", "Empat");
    p5 = tambah(p1, "P5", "Lima");

    cek(root->firstChild == p1 && p1->nextSibling == p2, "tambah anak urut");
    cek(p1->firstChild == p3 && p3->nextSibling == p5, "anak P1: P3 lalu P5");
    cek(p4->parent == p3 && p3->parent == p1 && p1->parent == root, "parent terhubung");

    cek(findPage(root, id) == p1, "findPage P1");
    cek(findPage(root, kata("P4")) == p4, "findPage cucu");
    cek(findPage(root, kata("P5")) == p5, "findPage anak kedua");
    cek(findPage(root, kata("P0")) == root, "findPage akar");
    cek(findPage(root, kata("P99")) == NULL, "findPage tidak ada");
    cek(findPage(NULL, id) == NULL, "findPage pohon kosong");

    cek(countPages(root) == 6, "countPages seluruh pohon");
    cek(countPages(p1) == 4, "countPages subtree P1");
    cek(countPages(p2) == 1, "countPages daun");
    cek(countPages(NULL) == 0, "countPages NULL");

    cek(getDepth(root) == 3, "kedalaman akar");
    cek(getDepth(p1) == 2, "kedalaman P1");
    cek(getDepth(p3) == 1, "kedalaman P3");
    cek(getDepth(p2) == 0 && getDepth(p4) == 0, "kedalaman daun");
    cek(getDepth(NULL) == 0, "kedalaman NULL");

    cek(isSubtree(p1, p4), "P4 ada di bawah P1");
    cek(isSubtree(p1, p1), "pohon termasuk dirinya sendiri");
    cek(isSubtree(root, p5), "P5 ada di bawah akar");
    cek(!isSubtree(p1, p2), "P2 bukan di bawah P1");
    cek(!isSubtree(p3, p1), "parent bukan di bawah anak");
    cek(!isSubtree(p3, p5), "saudara bukan di bawah saudara");
    cek(!isSubtree(NULL, p1) && !isSubtree(p1, NULL), "isSubtree NULL");

    cek(findLastSibling(root->firstChild) == p2, "findLastSibling akar");
    cek(findLastSibling(p1->firstChild) == p5, "findLastSibling P1");
    cek(findLastSibling(NULL) == NULL, "findLastSibling NULL");
    cek(findPrevSibling(root->firstChild, p2) == p1, "findPrevSibling P2");
    cek(findPrevSibling(root->firstChild, p1) == NULL, "findPrevSibling anak pertama");
    cek(findPrevSibling(NULL, p1) == NULL, "findPrevSibling NULL");

    tambahBlok(p3, 1, BlokText, "Satu");
    tambahBlok(p3, 2, BlokCheck, "Dua");
    tambahBlok(p5, 3, BlokText, "Tiga");
    cek(countBlocks(p3) == 2, "countBlocks P3");
    cek(countBlocks(p1) == 3, "countBlocks P1 dengan subtree");
    cek(countBlocks(root) == 3, "countBlocks seluruh pohon");
    cek(countBlocks(p2) == 0, "countBlocks tanpa blok");
    cek(countBlocks(NULL) == 0, "countBlocks NULL");

    printf("Tampilan pohon (cek manual):\n");
    printTree(root, 0);

    root->firstChild = p1->nextSibling;
    p1->nextSibling = NULL;
    deleteSubtree(p1);
    cek(findPage(root, kata("P4")) == NULL, "subtree terhapus");
    cek(findPage(root, kata("P2")) == p2, "saudara tetap ada");
    cek(countPages(root) == 2, "countPages setelah hapus subtree");
    deleteSubtree(NULL);

    deleteSubtree(root);

    printf("Tree driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}