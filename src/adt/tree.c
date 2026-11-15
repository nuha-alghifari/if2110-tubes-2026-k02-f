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
        printf(" ");
    }
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

// harusnya udah bener, tapi masih perlu dites
int countPages(AddressPage root){
    if (root == NULL) {
        return 0;
    }

    return 1 + countPages(root->firstChild) + countPages(root->nextSibling);

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
void printTree(AddressPage root, int level){
    if (root == NULL){
        return;
    }

    printIndent(level);
    printf("- %s - %s\n", root->pageId.tab, root->title.tab);
    printTree(root->firstChild, level + 1);
    printTree(root->nextSibling, level);
}

// TBA, masih perlu perbaikan dari versi sebelumnya
void deleteSubtree(AddressPage root){

}





