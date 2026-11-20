#ifndef PAGE_CONTROLLER_H
#define PAGE_CONTROLLER_H

void perintahPageAdd(AddressPage root, Kata parentId, Kata title);
void perintahPageTitle(AddressPage root, Kata targetId, Kata newTitle);
void perintahPageView(AddressPage root, Kata targetId);
bool PageAdd(AddressPage root, Kata parentId, Kata title);
bool PageTitle(AddressPage root, Kata pageId, Kata newTitle);
void PageView(AddressPage root, Kata targetId);
void Tree(AddressPage root);                
void Path(AddressPage root);               
void PageStats(AddressPage root);           
bool PageMove(AddressPage root);        
bool PageDelete(AddressPage root);    

#endif