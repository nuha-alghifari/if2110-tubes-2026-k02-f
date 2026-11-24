#include <stdio.h>
#include <stdbool.h>
#include "tree.h"
#include "page.h"
#include "kata.h"
#include "block_cmd.h"


int idCounter = 0;

// ambil angka ID
static int getPageId(Kata id) {
    int num = 0;
    for (int i = 1; i < id.length; i++) {
        if (id.tab[i] >= '0' && id.tab[i] <= '9') {
            num = num * 10 + (id.tab[i] - '0');
        }
    }
    return num;
}

// tulis ulang ID
static void setPageId(AddressPage node, int num) {
    char buf[16];
    int len = sprintf(buf, "P%d", num);
    node->pageId.length = len;
    for (int i = 0; i <= len; i++) { 
        node->pageId.tab[i] = buf[i];
    }
}

static int countSmallerId(AddressPage delTree, int threshold) {
    if (delTree == NULL) {
        return 0;
    }
    int self = (getPageId(delTree->pageId) < threshold) ? 1 : 0;
    return self + countSmallerId(delTree->firstChild, threshold) + countSmallerId(delTree->nextSibling, threshold);
}

static void changeForestId(AddressPage node, AddressPage deletedRoot) {
    if (node == NULL) {
        return;
    }
    int oldNum = getPageId(node->pageId);
    int change = countSmallerId(deletedRoot, oldNum);
    if (change > 0) {
        setPageId(node, oldNum - change);
    }
    changeForestId(node->firstChild, deletedRoot);
    changeForestId(node->nextSibling, deletedRoot);
}

bool PageAdd(AddressPage root, Kata parentId, Kata title){
    if (title.length < 1){
        printf("Judul halaman minimal 1 karakter.\n");
        return false;
    }

    AddressPage parent = findPage(root, parentId);
    if (parent == NULL){
        printf("Halaman tidak dapat dibuat: parent %s tidak ditemukan.\n", parentId.tab);
        return false;
    }

    // buat array sementara untuk menampung id
    char buf[16];
    int len = sprintf(buf, "P%d", idCounter + 1);

    Kata newId;
    newId.length = len;
    newId.tab[len] = '\0';
    // menyalin id dari buf ke tab dari newId
    for (int i = 0; i < len; i ++){
        newId.tab[i] = buf[i];
    }

    AddressPage newPage = createPage(newId, title);
    if (newPage == NULL) {
        return false;
    }
    idCounter++;

    newPage->parent = parent;
    if (parent->firstChild == NULL){
        parent->firstChild = newPage;
    } else{
        AddressPage lastSibling = findLastSibling(parent->firstChild);        
        lastSibling->nextSibling = newPage;
    }
    
    printf("Halaman %s berhasil dibuat.\n", newId.tab);
    return true;

}

// helper untuk mencetak lokasi halaman pada PageTitle (digunakan kembali di Path karena logikanya sama)
static void printLokasi(AddressPage node){
    if (node == NULL){
        return;
    }
    if (node->parent != NULL){
        printLokasi(node->parent);
        printf(" / ");
    }
    printf("%s", node->title.tab);
}


bool PageTitle(AddressPage root, Kata targetId, Kata newTitle){
    if (newTitle.length < 1){
        printf("Judul halaman minimal 1 karakter.\n");
        return false;
    }

    AddressPage target = findPage(root, targetId);
    if (target == NULL){
        printf("Halaman %s tidak dapat ditemukan.\n", targetId.tab);
        return false;
    }

    target->title = newTitle;
    printf("Judul halaman %s berhasil diubah.\n", targetId.tab);
    return true;
} 

// helper untuk mencetak subhalaman pada PageView
static void printDaftarSubHalaman(AddressPage child){
    if (child == NULL){
        return;
    }
    printf("- [%s] %s\n", child->pageId.tab, child->title.tab);
    printDaftarSubHalaman(child->nextSibling);
}

void PageView(AddressPage root, Kata targetId){
    AddressPage target = findPage(root, targetId);
    if (target == NULL){
        printf("Halaman %s tidak dapat ditemukan.\n", targetId.tab);
        return;
    }
    printf("[%s] %s\n", target->pageId.tab, target->title.tab);

    if(target->parent == NULL){
        printf("Parent: -\n");
    }else {
        printf("Parent: %s\n", target->parent->pageId.tab);
    }
    printf("Lokasi: ");
    printLokasi(target);
    printf("\n");

    printf("Tag: \n"); // menunggu implementasi tag

    printf("Blok: \n");
    CetakDaftarBlok(&target->blocks);

    if (target->firstChild == NULL){
        printf("Subhalaman: (kosong)\n");
    } else{
        printf("Subhalaman: \n");
        printDaftarSubHalaman(target->firstChild);
    }

} 

void Tree(AddressPage root, Kata targetId){
    AddressPage target = findPage(root, targetId);
    if (target == NULL){
        printf("Halaman %s tidak dapat ditemukan.\n", targetId.tab);
        return;
    }
    // bener ga kalo dia bukan P0 indentasi awalny tetep gaada?
    printTree(target, 0);
    printf("\n");
}      

void Path(AddressPage root, Kata targetId){
    AddressPage target = findPage(root, targetId);
    if (target == NULL){
        printf("Halaman %s tidak dapat ditemukan.\n", targetId.tab);
        return;
    }
    printLokasi(target);
    printf("\n");
}     

void PageStats(AddressPage root, Kata targetId){
    AddressPage target = findPage(root, targetId);
    if (target == NULL){
        printf("Halaman %s tidak dapat ditemukan.\n", targetId.tab);
        return;
    }
    printf("Halaman: %d\n", countPages(target));
    printf("Blok: %d\n", countBlocks(target));
    printf("Checklist: (TBA)\n"); //TBA abis implementasi fiturnya don
    printf("Kedalaman: %d\n", getDepth(target));
}

bool PageMove(AddressPage root, Kata targetId, Kata parentId){
    AddressPage target = findPage(root, targetId);
    if (target == NULL){
        printf("Halaman %s tidak dapat ditemukan.\n", targetId.tab);
        return false;
    }
    if (target == root || target->parent == NULL){
        printf("Root tidak bisa dipindah.");
        return false;
    }
    AddressPage newParent = findPage(root, parentId);
    if (newParent == NULL){
        printf("Parent %s tidak dapat ditemukan.\n", parentId.tab);
        return false;
    }
    if (target == newParent){
        printf("Parent baru tidak boleh sama dengan halaman.");
        return false;
    }
    if (isSubtree(target, newParent)){
        printf("Subtree dari halaman tidak boleh menjadi parent dari halaman tersebut.");
        return false;
    }

    if (target->parent == newParent) {
        printf("Halaman %s sudah berada di bawah %s.\n", targetId.tab, parentId.tab);
        return false;
    }

    AddressPage oldParent = target->parent;
    if (oldParent->firstChild == target){
        oldParent->firstChild = target->nextSibling;
    } else {
        AddressPage prev = findPrevSibling(oldParent->firstChild, target);
        if (prev != NULL) {
            prev->nextSibling = target->nextSibling;
        }
    }

    AddressPage temp = newParent->firstChild;
    target->parent = newParent;
    newParent->firstChild = target;
    target->nextSibling = temp;

    printf("Halaman %s berhasil dipindahkan ke %s.\n", targetId.tab, parentId.tab);
    return true;
}

bool PageDelete(AddressPage root, Kata targetId){
    AddressPage target = findPage(root, targetId);
    if (target == NULL){
        printf("Halaman %s tidak dapat ditemukan.\n", targetId.tab);
        return false;
    }

    if (target == root){
        printf("Root tidak boleh dihapus");
        return false;
    }

    AddressPage delParent = target->parent;
    if (delParent->firstChild == target){
        delParent->firstChild = target->nextSibling;
    } else {
        AddressPage prev = findPrevSibling(delParent->firstChild, target);
        if (prev != NULL){
            prev->nextSibling = target->nextSibling;
        }
    }
    target->nextSibling = NULL;

    int deletedPages = countPages(target);
    int deletedBlocks = countBlocks(target);
    changeForestId(root->firstChild, target);
    idCounter -= deletedPages;
    deleteSubtree(target);

    printf("%d halaman dan %d blok berhasil dihapus.\n", deletedPages, deletedBlocks);
    return true;
}




void perintahPageAdd(AddressPage root, Kata parentId, Kata title){
    PageAdd(root, parentId, title);
}

void perintahPageTitle(AddressPage root, Kata targetId, Kata newTitle){
    PageTitle(root, targetId, newTitle);
}

void perintahPageView(AddressPage root, Kata targetId){
    PageView(root, targetId);
}

void perintahTree(AddressPage root, Kata targetId){
    Tree(root, targetId);
}

void perintahPath(AddressPage root, Kata targetId){
    Path(root, targetId);
}

void perintahPageStats(AddressPage root, Kata targetId){
    PageStats(root, targetId);
}

void perintahPageMove(AddressPage root, Kata targetId, Kata parentId){
    PageMove(root, targetId, parentId);
}

void perintahPageDelete(AddressPage root, Kata targetId){
    PageDelete(root, targetId);
}
     

