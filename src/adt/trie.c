#include <stdlib.h>
#include "trie.h"

static unsigned char HurufKecil(unsigned char c) {
    if (c >= 'A' && c <= 'Z') {
        return (unsigned char)(c - 'A' + 'a');
    }
    return c;
}

static AddressTrie BuatNode(unsigned char huruf) {
    AddressTrie node = (AddressTrie)malloc(sizeof(TrieNode));
    if (node == NULL) return NULL;
    node->huruf = huruf;
    node->daftarId = NULL;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

static void HapusDaftarId(AddressTrieID p) {
    AddressTrieID temp;
    while (p != NULL) {
        temp = p;
        p = p->next;
        free(temp);
    }
}

static void HapusSubtree(AddressTrie node) {
    if (node == NULL) return;
    HapusSubtree(node->firstChild);
    HapusSubtree(node->nextSibling);
    HapusDaftarId(node->daftarId);
    free(node);
}

static AddressTrie CariAnak(AddressTrie parent, unsigned char huruf) {
    AddressTrie curr = parent->firstChild;
    while (curr != NULL && curr->huruf < huruf) {
        curr = curr->nextSibling;
    }
    if (curr != NULL && curr->huruf == huruf) {
        return curr;
    }
    return NULL;
}

static AddressTrie CariAtauBuatAnak(AddressTrie parent, unsigned char huruf) {
    AddressTrie prev = NULL;
    AddressTrie curr = parent->firstChild;
    AddressTrie baru;

    while (curr != NULL && curr->huruf < huruf) {
        prev = curr;
        curr = curr->nextSibling;
    }
    if (curr != NULL && curr->huruf == huruf) {
        return curr;
    }

    baru = BuatNode(huruf);
    if (baru == NULL) return NULL;
    baru->nextSibling = curr;
    if (prev == NULL) {
        parent->firstChild = baru;
    } else {
        prev->nextSibling = baru;
    }
    return baru;
}

static bool TambahId(AddressTrieID *head, int id) {
    AddressTrieID prev = NULL;
    AddressTrieID curr = *head;
    AddressTrieID baru;

    while (curr != NULL && curr->id < id) {
        prev = curr;
        curr = curr->next;
    }
    if (curr != NULL && curr->id == id) {
        return true;
    }

    baru = (AddressTrieID)malloc(sizeof(TrieIDNode));
    if (baru == NULL) return false;
    baru->id = id;
    baru->next = curr;
    if (prev == NULL) {
        *head = baru;
    } else {
        prev->next = baru;
    }
    return true;
}

static bool HapusId(AddressTrieID *head, int id) {
    AddressTrieID prev = NULL;
    AddressTrieID curr = *head;

    while (curr != NULL && curr->id < id) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL || curr->id != id) {
        return false;
    }
    if (prev == NULL) {
        *head = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr);
    return true;
}

void InitTrie(Trie *t) {
    t->root = BuatNode(0);
}

void HapusTrie(Trie *t) {
    if (t == NULL) return;
    HapusSubtree(t->root);
    t->root = NULL;
}

bool TrieKosong(const Trie *t) {
    if (t == NULL || t->root == NULL) return true;
    return t->root->firstChild == NULL && t->root->daftarId == NULL;
}

bool TrieTambah(Trie *t, const char *kunci, int id) {
    AddressTrie curr;
    int i;

    if (t == NULL || kunci == NULL) return false;
    if (t->root == NULL) {
        t->root = BuatNode(0);
        if (t->root == NULL) return false;
    }

    curr = t->root;
    for (i = 0; kunci[i] != '\0'; i++) {
        unsigned char c = HurufKecil((unsigned char)kunci[i]);
        curr = CariAtauBuatAnak(curr, c);
        if (curr == NULL) return false;
    }
    return TambahId(&curr->daftarId, id);
}

static bool HapusRek(AddressTrie node, const char *kunci, int id, bool *dihapus) {
    unsigned char c;
    AddressTrie prev = NULL;
    AddressTrie curr;

    if (*kunci == '\0') {
        *dihapus = HapusId(&node->daftarId, id);
        return node->daftarId == NULL && node->firstChild == NULL;
    }

    c = HurufKecil((unsigned char)*kunci);
    curr = node->firstChild;
    while (curr != NULL && curr->huruf < c) {
        prev = curr;
        curr = curr->nextSibling;
    }
    if (curr == NULL || curr->huruf != c) {
        return false;
    }

    if (HapusRek(curr, kunci + 1, id, dihapus)) {
        if (prev == NULL) {
            node->firstChild = curr->nextSibling;
        } else {
            prev->nextSibling = curr->nextSibling;
        }
        free(curr);
    }
    return node->daftarId == NULL && node->firstChild == NULL;
}

bool TrieHapus(Trie *t, const char *kunci, int id) {
    bool dihapus = false;
    if (t == NULL || t->root == NULL || kunci == NULL) return false;
    HapusRek(t->root, kunci, id, &dihapus);
    return dihapus;
}

static int HitungIdSubtree(AddressTrie node) {
    int total = 0;
    AddressTrieID p;
    AddressTrie anak;

    if (node == NULL) return 0;
    for (p = node->daftarId; p != NULL; p = p->next) {
        total++;
    }
    for (anak = node->firstChild; anak != NULL; anak = anak->nextSibling) {
        total += HitungIdSubtree(anak);
    }
    return total;
}

static void KumpulIdSubtree(AddressTrie node, int *buf, int *idx) {
    AddressTrieID p;
    AddressTrie anak;

    if (node == NULL) return;
    for (p = node->daftarId; p != NULL; p = p->next) {
        buf[*idx] = p->id;
        (*idx)++;
    }
    for (anak = node->firstChild; anak != NULL; anak = anak->nextSibling) {
        KumpulIdSubtree(anak, buf, idx);
    }
}

int TrieCariPrefix(const Trie *t, const char *prefix, int **hasil) {
    AddressTrie curr;
    int i, n, idx = 0;

    *hasil = NULL;
    if (t == NULL || t->root == NULL || prefix == NULL) return 0;

    curr = t->root;
    for (i = 0; prefix[i] != '\0'; i++) {
        unsigned char c = HurufKecil((unsigned char)prefix[i]);
        curr = CariAnak(curr, c);
        if (curr == NULL) return 0;
    }

    n = HitungIdSubtree(curr);
    if (n == 0) return 0;

    *hasil = (int *)malloc(sizeof(int) * n);
    if (*hasil == NULL) return -1;

    KumpulIdSubtree(curr, *hasil, &idx);
    return idx;
}
