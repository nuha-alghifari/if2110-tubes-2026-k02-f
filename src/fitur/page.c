#include <stdio.h>
#include <stdbool.h>
#include "tree.h"
#include "page.h"
#include "kata.h"
#include "block_cmd.h"


int idCounter = 0;

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

// helper untuk mencetak lokasi halaman pada PageTitle
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

void perintahPageAdd(AddressPage root, Kata parentId, Kata title){
    PageAdd(root, parentId, title);
}

void perintahPageTitle(AddressPage root, Kata targetId, Kata newTitle){
    PageTitle(root, targetId, newTitle);
}

void perintahPageView(AddressPage root, Kata targetId){
    PageView(root, targetId);
}

// void Tree(AddressPage root);                
// void Path(AddressPage root);               
// void PageStats(AddressPage root);           
// boolean PageMove(AddressPage root);        
// boolean PageDelete(AddressPage root);    

