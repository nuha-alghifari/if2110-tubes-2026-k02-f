#include <stdio.h>
#include "linkedlistPage.h"
#include "linkedlistTag.h"

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

static bool isiPage(LinkedlistPage l, const int *harapan, int n){
    int i = 0;
    AddresslistPage p = l;
    while (p != NULL && i < n){
        if (p->info != harapan[i]) return false;
        p = p->next;
        i++;
    }
    return p == NULL && i == n;
}

static bool isiTag(LinkedlistTag l, const char *const *harapan, int n){
    int i = 0;
    AddresslistTag p = l;
    while (p != NULL && i < n){
        Tag t = buatTag(harapan[i], 0);
        if (!isTagSama(p->info, t)) return false;
        p = p->next;
        i++;
    }
    return p == NULL && i == n;
}

static void ujiPage(void){
    LinkedlistPage l;
    AddresslistPage n;
    int a1[] = {5};
    int a3[] = {2, 5, 9};
    int a4[] = {1, 2, 5, 9};
    int a5[] = {1, 5, 9};
    int a6[] = {5, 9};
    int a7[] = {5};
    int i;

    createListPage(&l);
    cek(l == NULL, "page: create kosong");
    cek(lengthListPage(l) == 0, "page: panjang kosong");
    cek(searchPage(l, 1) == NULL, "page: cari di list kosong");
    deleteLastPage(&l);
    cek(l == NULL, "page: deleteLast list kosong");
    deleteAtPage(&l, 1);
    cek(l == NULL, "page: deleteAt list kosong");

    n = newNodePage(42);
    cek(n != NULL && n->info == 42 && n->next == NULL, "page: newNode");
    free(n);

    insertSortedPage(&l, 5);
    cek(isiPage(l, a1, 1), "page: insert pertama");
    insertSortedPage(&l, 9);
    insertSortedPage(&l, 2);
    cek(lengthListPage(l) == 3, "page: panjang 3");
    cek(isiPage(l, a3, 3), "page: urut menaik");

    insertSortedPage(&l, 1);
    cek(isiPage(l, a4, 4), "page: insert di depan");
    cek(searchPage(l, 5) != NULL && searchPage(l, 5)->info == 5, "page: cari ketemu");
    cek(searchPage(l, 7) == NULL, "page: cari tidak ketemu");

    deleteAtPage(&l, 2);
    cek(isiPage(l, a5, 3), "page: hapus tengah");
    deleteAtPage(&l, 1);
    cek(isiPage(l, a6, 2), "page: hapus kepala");
    deleteAtPage(&l, 77);
    cek(isiPage(l, a6, 2), "page: hapus tidak ada");
    deleteLastPage(&l);
    cek(isiPage(l, a7, 1), "page: deleteLast");
    deleteLastPage(&l);
    cek(l == NULL, "page: deleteLast sampai kosong");

    for (i = 10; i >= 1; i--){
        insertSortedPage(&l, i);
    }
    cek(lengthListPage(l) == 10, "page: 10 elemen");
    cek(l->info == 1, "page: elemen terkecil di depan");
    deleteAllPage(&l);
    cek(l == NULL && lengthListPage(l) == 0, "page: deleteAll");
    deleteAllPage(&l);
    cek(l == NULL, "page: deleteAll list kosong");
    cek(isBeforePage(1, 2) && !isBeforePage(2, 1) && !isBeforePage(2, 2), "page: isBefore");
}

static void ujiTag(void){
    LinkedlistTag l;
    AddresslistTag n;
    Tag t;
    const char *a1[] = {"kucing"};
    const char *a3[] = {"anjing", "burung", "kucing"};
    const char *a4[] = {"anjing", "ayam", "burung", "kucing"};
    const char *a5[] = {"anjing", "ayam", "kucing"};
    const char *a6[] = {"ayam", "kucing"};
    const char *a7[] = {"ayam"};

    createListTag(&l);
    cek(l == NULL, "tag: create kosong");
    cek(lengthListTag(l) == 0, "tag: panjang kosong");
    t = buatTag("kucing", 1);
    cek(searchTag(l, t) == NULL, "tag: cari di list kosong");
    deleteLastTag(&l);
    cek(l == NULL, "tag: deleteLast list kosong");
    deleteAtTag(&l, t);
    cek(l == NULL, "tag: deleteAt list kosong");

    n = newNodeTag(t);
    cek(n != NULL && n->next == NULL && n->info.id == 1, "tag: newNode");
    free(n);

    insertSortedTag(&l, buatTag("kucing", 1));
    cek(isiTag(l, a1, 1), "tag: insert pertama");
    insertSortedTag(&l, buatTag("burung", 2));
    insertSortedTag(&l, buatTag("anjing", 3));
    cek(isiTag(l, a3, 3), "tag: urut alfabet");
    cek(lengthListTag(l) == 3, "tag: panjang 3");

    insertSortedTag(&l, buatTag("ayam", 4));
    cek(isiTag(l, a4, 4), "tag: insert di tengah");
    t = buatTag("burung", 0);
    cek(searchTag(l, t) != NULL && searchTag(l, t)->info.id == 2, "tag: cari ketemu menurut nama");
    t = buatTag("ikan", 0);
    cek(searchTag(l, t) == NULL, "tag: cari tidak ketemu");

    deleteAtTag(&l, buatTag("burung", 0));
    cek(isiTag(l, a5, 3), "tag: hapus tengah");
    deleteAtTag(&l, buatTag("anjing", 0));
    cek(isiTag(l, a6, 2), "tag: hapus kepala");
    deleteAtTag(&l, buatTag("ikan", 0));
    cek(isiTag(l, a6, 2), "tag: hapus tidak ada");
    deleteLastTag(&l);
    cek(isiTag(l, a7, 1), "tag: deleteLast");
    deleteLastTag(&l);
    cek(l == NULL, "tag: deleteLast sampai kosong");

    insertSortedTag(&l, buatTag("a", 1));
    insertSortedTag(&l, buatTag("ab", 2));
    insertSortedTag(&l, buatTag("b", 3));
    cek(l->info.id == 1 && l->next->info.id == 2 && l->next->next->info.id == 3, "tag: prefix pendek duluan");
    deleteAllTag(&l);
    cek(l == NULL && lengthListTag(l) == 0, "tag: deleteAll");
    deleteAllTag(&l);
    cek(l == NULL, "tag: deleteAll list kosong");
}

int main(void){
    ujiPage();
    ujiTag();

    printf("Linked list driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}