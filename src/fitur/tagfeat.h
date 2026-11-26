#ifndef TAGFEAT_H
#define TAGFEAT_H
#include "jurnal.h"

void CMDTagAdd(Jurnal *J);
void CMDTagDelete(Jurnal *J);
void CMDTagList(Jurnal *J);
void CMDTag(Jurnal *J);
void CMDUntag(Jurnal *J);
void CMDPageTags(Jurnal *J);
void CMDTagPages(Jurnal *J);
void TagTambahHalaman(Jurnal *J, int idHalaman);
void TagHapusHalaman(Jurnal *J, int idHalaman);

#endif