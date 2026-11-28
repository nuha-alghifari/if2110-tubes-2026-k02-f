#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "jurnal.h"
#include "page.h"

static int total = 0;
static int gagal = 0;

static void cek(bool kondisi, const char *nama) {
    total++;
    if (!kondisi) {
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

static bool mengandungId(const int *ids, int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (ids[i] == target) return true;
    }
    return false;
}

static void ujiDasar(void) {
    Trie t;
    int *hasil = NULL;
    int n;

    InitTrie(&t);
    cek(TrieKosong(&t), "trie baru kosong");
    cek(TrieCariPrefix(&t, "por", &hasil) == 0 && hasil == NULL, "cari di trie kosong");

    cek(TrieTambah(&t, "Portal Kampus", 3), "tambah Portal Kampus");
    cek(TrieTambah(&t, "Portal Timur", 4), "tambah Portal Timur");
    cek(TrieTambah(&t, "Jurnal Anomali", 1), "tambah Jurnal Anomali");
    cek(!TrieKosong(&t), "trie tidak kosong setelah tambah");

    n = TrieCariPrefix(&t, "Por", &hasil);
    cek(n == 2 && mengandungId(hasil, n, 3) && mengandungId(hasil, n, 4), "cari prefix Por");
    free(hasil);

    n = TrieCariPrefix(&t, "PORTAL K", &hasil);
    cek(n == 1 && hasil[0] == 3, "cari prefix abaikan kapital");
    free(hasil);

    n = TrieCariPrefix(&t, "Kampus", &hasil);
    cek(n == 0 && hasil == NULL, "bukan prefix tidak cocok");

    HapusTrie(&t);
    cek(TrieKosong(&t), "trie kosong setelah hapus");
}

static void ujiJudulSamaDanHapus(void) {
    Trie t;
    int *hasil = NULL;
    int n;

    InitTrie(&t);
    TrieTambah(&t, "Catatan", 1);
    TrieTambah(&t, "catatan", 5);
    TrieTambah(&t, "Catatan", 1);
    TrieTambah(&t, "Catatan Lama", 2);

    n = TrieCariPrefix(&t, "cat", &hasil);
    cek(n == 3 && mengandungId(hasil, n, 1) && mengandungId(hasil, n, 5) && mengandungId(hasil, n, 2),
        "judul sama beda kapital tersimpan semua tanpa duplikat ID");
    free(hasil);

    cek(TrieHapus(&t, "CATATAN", 1), "hapus ID 1 dari judul sama");
    cek(!TrieHapus(&t, "catatan", 1), "hapus ID yang sudah tidak ada");
    cek(!TrieHapus(&t, "tidakada", 9), "hapus kunci tidak ada");

    n = TrieCariPrefix(&t, "catatan", &hasil);
    cek(n == 2 && !mengandungId(hasil, n, 1) && mengandungId(hasil, n, 5) && mengandungId(hasil, n, 2),
        "ID lain tetap ada setelah satu ID dihapus");
    free(hasil);

    cek(TrieHapus(&t, "catatan", 5), "hapus ID 5");
    cek(TrieHapus(&t, "catatan lama", 2), "hapus ID 2");
    cek(TrieKosong(&t), "node dibersihkan sampai trie kosong");

    HapusTrie(&t);
}

static void ujiTagUrut(void) {
    Trie t;
    int *hasil = NULL;
    int n;

    InitTrie(&t);
    TrieTambah(&t, "rencana", 2);
    TrieTambah(&t, "anomali", 1);
    TrieTambah(&t, "analisis", 3);

    n = TrieCariPrefix(&t, "an", &hasil);
    cek(n == 2 && hasil[0] == 3 && hasil[1] == 1, "hasil prefix tag urut abjad");
    free(hasil);

    HapusTrie(&t);
}

static void ujiIntegrasiJurnal(void) {
    Jurnal j;
    char pesan[Max_Pesan];
    int *hasil = NULL;
    int n;
    Kata parentId, judul, targetId;

    InitJurnal(&j);
    cek(MuatJurnal(&j, "sample-data/jurnal", pesan, Max_Pesan), "muat sample-data/jurnal");

    n = TrieCariPrefix(&j.trieHalaman, "Por", &hasil);
    cek(n == 1 && hasil[0] == 3, "prefix halaman dari sample");
    free(hasil);

    n = TrieCariPrefix(&j.trieTag, "an", &hasil);
    cek(n == 1 && hasil[0] == 1, "prefix tag dari sample");
    free(hasil);

    KataDariC(&parentId, "P1");
    KataDariC(&judul, "Portal Selatan");
    cek(PageAdd(&j, parentId, judul), "PageAdd memperbarui trie");

    n = TrieCariPrefix(&j.trieHalaman, "portal", &hasil);
    cek(n == 2 && mengandungId(hasil, n, 3) && mengandungId(hasil, n, 4), "halaman baru muncul di trie");
    free(hasil);

    KataDariC(&targetId, "P1");
    cek(PageDelete(&j, targetId), "PageDelete membersihkan subtree dari trie");

    n = TrieCariPrefix(&j.trieHalaman, "portal", &hasil);
    cek(n == 0 && hasil == NULL, "halaman terhapus tidak muncul lagi di trie");

    n = TrieCariPrefix(&j.trieHalaman, "renc", &hasil);
    cek(n == 1 && hasil[0] == 2, "halaman lain tetap ada di trie");
    free(hasil);

    HapusJurnal(&j);
}

int main(void) {
    ujiDasar();
    ujiJudulSamaDanHapus();
    ujiTagUrut();
    ujiIntegrasiJurnal();

    printf("Trie driver: %d/%d lulus\n", total - gagal, total);
    return gagal == 0 ? 0 : 1;
}
