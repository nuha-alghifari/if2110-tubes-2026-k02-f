#include <stdio.h>
#include "queue.h"

static int total = 0;
static int gagal = 0;

static void cek(bool kondisi, const char *nama){
    total++;
    if (!kondisi){
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

int main(void){
    Queue q;
    int x = -1;
    int i;

    CreateQueue(&q);
    cek(IsQueueEmpty(q), "queue baru kosong");
    cek(QueueLength(q) == 0, "panjang queue baru");
    cek(!Dequeue(&q, &x), "dequeue queue kosong gagal");

    cek(Enqueue(&q, 10), "enqueue 10");
    cek(!IsQueueEmpty(q), "tidak kosong setelah enqueue");
    cek(QueueLength(q) == 1, "panjang 1");
    cek(Enqueue(&q, 20), "enqueue 20");
    cek(Enqueue(&q, 30), "enqueue 30");
    cek(QueueLength(q) == 3, "panjang 3");

    cek(Dequeue(&q, &x) && x == 10, "FIFO elemen pertama");
    cek(Dequeue(&q, &x) && x == 20, "FIFO elemen kedua");
    cek(QueueLength(q) == 1, "panjang 1 setelah dua dequeue");

    cek(Enqueue(&q, 40), "enqueue setelah dequeue");
    cek(Dequeue(&q, &x) && x == 30, "FIFO elemen ketiga");
    cek(Dequeue(&q, &x) && x == 40, "FIFO elemen keempat");
    cek(IsQueueEmpty(q), "kosong setelah semua keluar");
    cek(q.head == NULL && q.tail == NULL, "head dan tail NULL saat kosong");
    cek(!Dequeue(&q, &x), "dequeue kosong gagal lagi");

    for (i = 1; i <= 100; i++) Enqueue(&q, i);
    cek(QueueLength(q) == 100, "enqueue 100 elemen");
    DeleteAllQueue(&q);
    cek(IsQueueEmpty(q) && QueueLength(q) == 0, "DeleteAllQueue");
    cek(Enqueue(&q, 7) && Dequeue(&q, &x) && x == 7, "queue dapat dipakai lagi");

    printf("Total test: %d, gagal: %d\n", total, gagal);
    return gagal == 0 ? 0 : 1;
}