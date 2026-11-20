#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include "kata.h"
#include "block.h"

typedef struct TreeNode* AddressPage;
typedef struct TreeNode {
    Kata pageId;
    Kata title;
    AddressPage parent;
    AddressPage firstChild;
    AddressPage nextSibling;
    ListBlok blocks;
} TreeNode;


AddressPage findLastSibling(AddressPage parent);
AddressPage createPage(Kata id, Kata title);
void deallocatePage(AddressPage p);
AddressPage findPage(AddressPage root, Kata targetId);
int countPages(AddressPage root);
int getDepth(AddressPage root);
bool isSubtree(AddressPage parent, AddressPage target);
void printTree(AddressPage root, int level);
void deleteSubtree(AddressPage root);

#endif