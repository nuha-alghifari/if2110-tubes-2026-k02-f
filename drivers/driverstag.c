#include <stdio.h>
#include "tag.h"

static int total = 0;
static int gagal = 0;

static void cek(bool kondisi, const char *nama){
    total++;
    if (!kondisi){
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

static Tag buat(const char *s){
    Kata k;
    KataDariC(&k, s);
    return copyKata(k);
}

int main(void){
    Tag t, u;
    Kata k;
    int i;

    t = buat("anomali");
    cek(isTagValid(t), "valid huruf kecil");
    t = buat("a-b_1");
    cek(isTagValid(t), "valid angka, strip, underscore");
    t = buat("123");
    cek(isTagValid(t), "valid hanya angka");
    t = buat("");
    cek(!isTagValid(t), "kosong ditolak");
    t = buat("Anomali");
    cek(!isTagValid(t), "huruf kapital ditolak");
    t = buat("dua kata");
    cek(!isTagValid(t), "spasi ditolak");
    t = buat("tag;");
    cek(!isTagValid(t), "titik koma ditolak");
    t = buat("tag.1");
    cek(!isTagValid(t), "titik ditolak");
    t = buat("a\"b");
    cek(!isTagValid(t), "petik ditolak");

    KataDariC(&k, "portal");
    t = copyKata(k);
    cek(t.length == 6, "copyKata panjang");
    cek(t.kata[0] == 'p' && t.kata[5] == 'l', "copyKata isi");
    cek(t.kata[6] == '\0', "copyKata diakhiri null");
    cek(t.id == 0, "copyKata id awal 0");

    KosongKata(&k);
    for (i = 0; i < Max_Kata; i++){
        TambahKata(&k, 'a');
    }
    t = copyKata(k);
    cek(t.length == Max_Kata && t.kata[Max_Kata] == '\0', "copyKata kata penuh");

    t = buat("abc");
    u = buat("abc");
    cek(isTagSama(t, u), "sama persis");
    u.id = 7;
    cek(isTagSama(t, u), "id tidak ikut dibandingkan");
    u = buat("abd");
    cek(!isTagSama(t, u), "beda huruf terakhir");
    u = buat("abcd");
    cek(!isTagSama(t, u), "beda panjang");
    u = buat("xbc");
    cek(!isTagSama(t, u), "beda huruf pertama");

    t = buat("a");
    u = buat("b");
    cek(isBeforeTag(t, u), "a sebelum b");
    cek(!isBeforeTag(u, t), "b tidak sebelum a");
    t = buat("ab");
    u = buat("abc");
    cek(isBeforeTag(t, u), "prefix lebih pendek duluan");
    cek(!isBeforeTag(u, t), "prefix lebih panjang belakangan");
    t = buat("abc");
    u = buat("abc");
    cek(!isBeforeTag(t, u), "sama tidak sebelum");
    t = buat("abc");
    u = buat("abd");
    cek(isBeforeTag(t, u), "beda di huruf terakhir");
    t = buat("1");
    u = buat("a");
    cek(isBeforeTag(t, u), "angka sebelum huruf");

    printf("Tag driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}