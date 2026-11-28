#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>

typedef struct TrieIDNode *AddressTrieID;
typedef struct TrieIDNode {
    int id;
    AddressTrieID next;
} TrieIDNode;

typedef struct TrieNode *AddressTrie;
typedef struct TrieNode {
    unsigned char huruf;
    AddressTrieID daftarId;
    AddressTrie firstChild;
    AddressTrie nextSibling;
} TrieNode;

typedef struct {
    AddressTrie root;
} Trie;

void InitTrie(Trie *t);
void HapusTrie(Trie *t);
bool TrieKosong(const Trie *t);
bool TrieTambah(Trie *t, const char *kunci, int id);
bool TrieHapus(Trie *t, const char *kunci, int id);
int TrieCariPrefix(const Trie *t, const char *prefix, int **hasil);

#endif
