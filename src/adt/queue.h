#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>

typedef struct nodequeue *AddressQueue;
typedef struct nodequeue {
    int info;
    AddressQueue next;
} NodeQueue;

typedef struct {
    AddressQueue head; // elemen paling depan 
    AddressQueue tail; // elemen paling belakang
    int length;
} Queue;

void CreateQueue(Queue *q);          // buat queue kosong
bool IsQueueEmpty(Queue q);          // true kalau queue kosong
int QueueLength(Queue q);            // jumlah elemen
bool Enqueue(Queue *q, int x);       // masuk lewat belakang, false kalau alokasi gagal
bool Dequeue(Queue *q, int *x);      // keluar lewat depan, false kalau queue kosong
void DeleteAllQueue(Queue *q);       // bebaskan seluruh elemen, queue jadi kosong

#endif