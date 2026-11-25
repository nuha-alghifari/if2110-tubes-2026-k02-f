#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

    // char pageId[16];
    // char title[128];
    // AddressPage parent;
    // AddressPage firstChild;
    // AddressPage nextSibling;
    // AddressBlock blocks;


static bool isIdEqual(Kata a, Kata b){
    if (a.length != b.length){
        return false;
    }
    for (int i = 0; i < a.length; i++){
        if (a.tab[i] != b.tab[i]){
            return false;
        }
    }
    return true;
}


static bool findInForest(AddressPage node, AddressPage target){
    if (node == NULL){
        return false;
    }

    if (node == target){
        return true;
    }
    return findInForest(node->firstChild, target) || findInForest(node->nextSibling, target);
}

void printIndent(int level){
    int i;
    for (i = 0; i < level; i++){
        printf("  ");
    }
}

AddressPage findPrevSibling(AddressPage current, AddressPage target) {
    if (current == NULL || current->nextSibling == NULL){
        return NULL;
    }
    if (current->nextSibling == target){
        return current;
    }
    return findPrevSibling(current->nextSibling, target);
}

AddressPage findLastSibling(AddressPage child){
    if (child == NULL){
        return NULL;
    }
    if (child->nextSibling == NULL){
        return child;
    }
    return findLastSibling(child->nextSibling);
}

AddressPage createPage(Kata id, Kata title){
    AddressPage p = malloc(sizeof(TreeNode));
    if (p != NULL){
        p->pageId = id;
        p->title = title;
        
        p->parent = NULL;
        p->firstChild = NULL;
        p->nextSibling = NULL;
        BuatListBlok(&p->blocks);
    }

    return p;
}

void deallocatePage(AddressPage p){
    if (p != NULL){
        HapusSemuaBlok(&p->blocks);
        free(p);
    }
}

bool isSubtree(AddressPage parent, AddressPage target){
    if (parent == NULL || target == NULL) {
        return false;
    }

    if (parent == target){
        return true;
    }

    return findInForest(parent->firstChild, target);
}

AddressPage findPage(AddressPage root, Kata targetId){
    if (root == NULL) {
        return NULL;
    }

    if (isIdEqual(root->pageId, targetId)){
        return root;
    }

    AddressPage found = findPage(root->firstChild, targetId);
    if (found != NULL){
        return found;
    }

    return findPage(root->nextSibling, targetId);
}

static int countForestPages(AddressPage root){
    if (root == NULL){
        return 0;
    }
    return 1 + countForestPages(root->firstChild) + countForestPages(root->nextSibling);
}
// harusnya udah bener, tapi masih perlu dites
int countPages(AddressPage root){
    if (root == NULL) {
        return 0;
    }

    return 1 + countForestPages(root->firstChild);

}

// masih perlu dites
int getDepth(AddressPage root){
    if (root == NULL || root->firstChild == NULL){
        return 0;
    }

    int maxChildDepth = 0;
    AddressPage current = root->firstChild;

    while (current != NULL){
        int d = getDepth(current);
        if (d > maxChildDepth) {
            maxChildDepth = d;
        }
        current = current->nextSibling;
    }

    return 1 + maxChildDepth;
}

// masih perlu dites

static void printForest(AddressPage node, int level){
    if (node == NULL){
        return;
    }

    printIndent(level);
    printf("[%s] - %s\n", node->pageId.tab, node->title.tab);
    printForest(node->firstChild, level + 1);
    printForest(node->nextSibling, level);
}
void printTree(AddressPage node, int level){
    if (node == NULL){
        return;
    }

    printIndent(level);
    printf("[%s] - %s\n", node->pageId.tab, node->title.tab);
    printForest(node->firstChild, level + 1);
}

static int countForestBlocks(AddressPage node) {
    if (node == NULL) {
        return 0;
    }
    return JumlahBlok(&node->blocks) + countForestBlocks(node->firstChild) + countForestBlocks(node->nextSibling);
}

int countBlocks(AddressPage target) {
    if (target == NULL) {
        return 0;
    }
    return JumlahBlok(&target->blocks) + countForestBlocks(target->firstChild);
}

static void deleteForest(AddressPage node){
    if (node == NULL){
        return;
    }
    deleteForest(node->firstChild);
    deleteForest(node->nextSibling);
    deallocatePage(node);
}

void deleteSubtree(AddressPage root){
    if (root == NULL){
        return;
    }
    deleteForest(root->firstChild);
    deallocatePage(root);
}





