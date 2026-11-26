#ifndef PAGE_CMD_H
#define PAGE_CMD_H

#include "jurnal.h"

void CMDPageAdd(Jurnal *J);
void CMDPageTitle(Jurnal *J);
void CMDPageView(Jurnal *J);
void CMDTree(Jurnal *J);
void CMDPath(Jurnal *J);
void CMDPageStats(Jurnal *J);
void CMDPageMove(Jurnal *J);
void CMDPageDelete(Jurnal *J);

#endif