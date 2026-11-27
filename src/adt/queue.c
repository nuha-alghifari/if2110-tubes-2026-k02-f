#include <stdlib.h>
#include "queue.h"

void CreateQueue(Queue *q) {
    q->head = NULL;
    q->tail = NULL;
    q->length = 0;
}

bool IsQueueEmpty(Queue q) {
    return q.head == NULL;
}

int QueueLength(Queue q) {
    return q.length;
}

bool Enqueue(Queue *q, int x) {
    AddressQueue p = (AddressQueue)malloc(sizeof(NodeQueue));
    if (p == NULL) return false;
    p->info = x;
    p->next = NULL;
    if (q->tail == NULL) {
        q->head = p;
    } else {
        q->tail->next = p;
    }
    q->tail = p;
    q->length++;
    return true;
}

bool Dequeue(Queue *q, int *x) {
    AddressQueue p;
    if (q->head == NULL) return false;
    p = q->head;
    *x = p->info;
    q->head = p->next;
    if (q->head == NULL) q->tail = NULL;
    free(p);
    q->length--;
    return true;
}

void DeleteAllQueue(Queue *q) {
    int buang;
    while (Dequeue(q, &buang)) {
    }
}