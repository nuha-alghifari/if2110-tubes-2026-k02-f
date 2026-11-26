#ifndef LINKBACKLINK_H
#define LINKBACKLINK_H
#include "GraphPage.h"
#include "input.h"
#include "jurnal.h"

void CMDLink(Jurnal *j);
void CMDUnlink(Jurnal *j);
void CMDLinks(Jurnal *j);
void CMDBacklinks(Jurnal *j);
void LinkHapusHalaman(Jurnal *j, int idHalaman);

#endif