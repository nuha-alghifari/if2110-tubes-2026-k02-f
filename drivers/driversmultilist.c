#include <stdio.h>
#include "multilistPage.h"
#include "multilistTag.h"

static int total = 0;
static int gagal = 0;

static void cek(bool kondisi, const char *nama){
    total++;
    if (!kondisi){
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

static Tag buatTag(const char *s, int id){
    Kata k;
    Tag t;
    KataDariC(&k, s);
    t = copyKata(k);
    t.id = id;
    return t;
}

static int hitungNode(MultiListPage l){
    int n = 0;
    while (l != NULL){
        n++;
        l = l->next;
    }
    return n;
}

static int hitungNodeTag(MultiListTag l){
    int n = 0;
    while (l != NULL){
        n++;
        l = l->next;
    }
    return n;
}

static void ujiHalamanKeTag(void){
    MultiListPage l;
    AddressMultiListPage n;
    Tag anjing = buatTag("anjing", 1);
    Tag burung = buatTag("burung", 2);
    Tag kucing = buatTag("kucing", 3);

    CreateMultiListPage(&l);
    cek(l == NULL, "page: create kosong");
    cek(searchMLPage(l, 1) == NULL, "page: cari di list kosong");
    cek(searchMLPageByID(l, 1) == NULL, "page: cari ID di list kosong");
    deleteAtMLPage(&l, 1);
    cek(l == NULL, "page: hapus di list kosong");

    n = newNodeMLPage(7);
    cek(n != NULL && n->info == 7 && n->next == NULL && n->child == NULL, "page: newNode");
    free(n);

    insertLastMLPage(&l, 1);
    insertLastMLPage(&l, 2);
    insertLastMLPage(&l, 3);
    cek(hitungNode(l) == 3, "page: tiga node");
    cek(l->info == 1 && l->next->info == 2 && l->next->next->info == 3, "page: urutan sisip belakang");
    cek(l->child == NULL, "page: child awal kosong");
    cek(searchMLPage(l, 2) != NULL && searchMLPage(l, 2)->info == 2, "page: cari ketemu");
    cek(searchMLPageByID(l, 3) != NULL && searchMLPageByID(l, 3)->info == 3, "page: cari ID ketemu");
    cek(searchMLPage(l, 9) == NULL && searchMLPageByID(l, 9) == NULL, "page: cari tidak ketemu");

    insertLinkedListPage(&l, 2, kucing);
    insertLinkedListPage(&l, 2, anjing);
    insertLinkedListPage(&l, 2, burung);
    n = searchMLPage(l, 2);
    cek(lengthListTag(n->child) == 3, "page: tiga tag di P2");
    cek(n->child->info.id == 1 && n->child->next->info.id == 2 && n->child->next->next->info.id == 3, "page: tag urut alfabet");
    cek(l->child == NULL && l->next->next->child == NULL, "page: halaman lain tidak berubah");

    insertLinkedListPage(&l, 99, kucing);
    cek(hitungNode(l) == 3, "page: sisip tag ke halaman tidak ada diabaikan");

    insertLinkedListPage(&l, 1, kucing);
    cek(lengthListTag(searchMLPage(l, 1)->child) == 1, "page: tag di P1");

    deleteLinkedListPage(&l, 2, burung);
    n = searchMLPage(l, 2);
    cek(lengthListTag(n->child) == 2, "page: hapus satu tag");
    cek(searchTag(n->child, burung) == NULL, "page: tag terhapus");
    cek(searchTag(n->child, kucing) != NULL, "page: tag lain tetap");
    deleteLinkedListPage(&l, 2, burung);
    cek(lengthListTag(n->child) == 2, "page: hapus tag yang tidak ada");
    deleteLinkedListPage(&l, 99, kucing);
    cek(hitungNode(l) == 3, "page: hapus tag dari halaman tidak ada");

    deleteAtMLPage(&l, 2);
    cek(hitungNode(l) == 2 && searchMLPage(l, 2) == NULL, "page: hapus node tengah");
    cek(l->info == 1 && l->next->info == 3, "page: sisa urutan");
    deleteAtMLPage(&l, 1);
    cek(l->info == 3 && hitungNode(l) == 1, "page: hapus node kepala");
    deleteAtMLPage(&l, 99);
    cek(hitungNode(l) == 1, "page: hapus node tidak ada");
    deleteAtMLPage(&l, 3);
    cek(l == NULL, "page: hapus node terakhir");
}

static void ujiTagKeHalaman(void){
    MultiListTag l;
    AddressMultiListTag n;
    Tag anjing = buatTag("anjing", 1);
    Tag burung = buatTag("burung", 2);
    Tag kucing = buatTag("kucing", 3);
    Tag ikan = buatTag("ikan", 0);

    CreateMultiListTag(&l);
    cek(l == NULL, "tag: create kosong");
    cek(searchMLTag(l, anjing) == NULL, "tag: cari di list kosong");
    cek(searchMLTagByID(l, 1) == NULL, "tag: cari ID di list kosong");
    deleteAtMLTag(&l, anjing);
    cek(l == NULL, "tag: hapus di list kosong");

    n = newNodeMLTag(anjing);
    cek(n != NULL && n->info.id == 1 && n->next == NULL && n->child == NULL, "tag: newNode");
    free(n);

    insertLastMLTag(&l, anjing);
    insertLastMLTag(&l, burung);
    insertLastMLTag(&l, kucing);
    cek(hitungNodeTag(l) == 3, "tag: tiga node");
    cek(l->info.id == 1 && l->next->info.id == 2 && l->next->next->info.id == 3, "tag: urutan sisip belakang");
    cek(searchMLTag(l, burung) != NULL && searchMLTag(l, burung)->info.id == 2, "tag: cari menurut nama");
    cek(searchMLTagByID(l, 3) != NULL && searchMLTagByID(l, 3)->info.id == 3, "tag: cari menurut ID");
    cek(searchMLTag(l, ikan) == NULL && searchMLTagByID(l, 9) == NULL, "tag: cari tidak ketemu");

    insertLinkedListTag(&l, burung, 5);
    insertLinkedListTag(&l, burung, 2);
    insertLinkedListTag(&l, burung, 9);
    n = searchMLTag(l, burung);
    cek(lengthListPage(n->child) == 3, "tag: tiga halaman di tag burung");
    cek(n->child->info == 2 && n->child->next->info == 5 && n->child->next->next->info == 9, "tag: halaman terurut");
    cek(l->child == NULL && l->next->next->child == NULL, "tag: tag lain tidak berubah");

    insertLinkedListTag(&l, ikan, 1);
    cek(hitungNodeTag(l) == 3, "tag: sisip halaman ke tag tidak ada diabaikan");

    deleteLinkedListTag(&l, burung, 5);
    cek(lengthListPage(n->child) == 2 && searchPage(n->child, 5) == NULL, "tag: hapus satu halaman");
    deleteLinkedListTag(&l, burung, 5);
    cek(lengthListPage(n->child) == 2, "tag: hapus halaman yang tidak ada");
    deleteLinkedListTag(&l, ikan, 2);
    cek(hitungNodeTag(l) == 3, "tag: hapus halaman dari tag tidak ada");

    deleteAtMLTag(&l, burung);
    cek(hitungNodeTag(l) == 2 && searchMLTag(l, burung) == NULL, "tag: hapus node tengah");
    cek(l->info.id == 1 && l->next->info.id == 3, "tag: sisa urutan");
    deleteAtMLTag(&l, anjing);
    cek(l->info.id == 3 && hitungNodeTag(l) == 1, "tag: hapus node kepala");
    deleteAtMLTag(&l, ikan);
    cek(hitungNodeTag(l) == 1, "tag: hapus node tidak ada");
    deleteAtMLTag(&l, kucing);
    cek(l == NULL, "tag: hapus node terakhir");
}

static void ujiDuaArah(void){
    MultiListPage halaman;
    MultiListTag tag;
    Tag anomali = buatTag("anomali", 1);
    Tag rencana = buatTag("rencana", 2);

    CreateMultiListPage(&halaman);
    CreateMultiListTag(&tag);
    insertLastMLPage(&halaman, 1);
    insertLastMLPage(&halaman, 2);
    insertLastMLPage(&halaman, 3);
    insertLastMLTag(&tag, anomali);
    insertLastMLTag(&tag, rencana);

    insertLinkedListPage(&halaman, 1, anomali);
    insertLinkedListTag(&tag, anomali, 1);
    insertLinkedListPage(&halaman, 2, rencana);
    insertLinkedListTag(&tag, rencana, 2);
    insertLinkedListPage(&halaman, 3, anomali);
    insertLinkedListTag(&tag, anomali, 3);

    cek(lengthListPage(searchMLTag(tag, anomali)->child) == 2, "dua arah: anomali dipakai dua halaman");
    cek(lengthListTag(searchMLPage(halaman, 3)->child) == 1, "dua arah: P3 punya satu tag");

    deleteLinkedListPage(&halaman, 1, anomali);
    deleteLinkedListTag(&tag, anomali, 1);
    cek(searchPage(searchMLTag(tag, anomali)->child, 1) == NULL, "dua arah: P1 lepas dari anomali");
    cek(searchMLPage(halaman, 1)->child == NULL, "dua arah: P1 tidak punya tag");

    deleteAtMLPage(&halaman, 2);
    deleteLinkedListTag(&tag, rencana, 2);
    cek(searchMLPage(halaman, 2) == NULL, "dua arah: P2 dihapus");
    cek(searchMLTag(tag, rencana)->child == NULL, "dua arah: rencana tanpa halaman");

    deleteAtMLTag(&tag, anomali);
    deleteAtMLTag(&tag, rencana);
    deleteAtMLPage(&halaman, 1);
    deleteAtMLPage(&halaman, 3);
    cek(tag == NULL && halaman == NULL, "dua arah: semua kosong");
}

int main(void){
    ujiHalamanKeTag();
    ujiTagKeHalaman();
    ujiDuaArah();

    printf("Multilist driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}