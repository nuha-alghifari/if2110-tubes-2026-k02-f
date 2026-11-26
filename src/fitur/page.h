#ifndef PAGE_CONTROLLER_H
#define PAGE_CONTROLLER_H
#include <stdbool.h>
#include "tree.h"
#include "kata.h"
#include "jurnal.h"

void perintahPageAdd(Jurnal *J, Kata parentId, Kata title);
void perintahPageTitle(AddressPage root, Kata targetId, Kata newTitle);
void perintahPageView(AddressPage root, Kata targetId);
bool PageAdd(Jurnal *J, Kata parentId, Kata title);
bool PageTitle(AddressPage root, Kata pageId, Kata newTitle);
void PageView(AddressPage root, Kata targetId);
void Tree(AddressPage root, Kata targetId);                
void Path(AddressPage root, Kata targetId);               
void PageStats(AddressPage root, Kata targetId);           
bool PageMove(AddressPage root, Kata targetId, Kata parentId);        
bool PageDelete(Jurnal *J, Kata targetId);    
void perintahTree(AddressPage root, Kata targetId);
void perintahPath(AddressPage root, Kata targetId);

#endif