#ifndef HANDLER_H
#define HANDLER_H
#include "jurnal.h"

typedef void (*Handler)(Jurnal *J);

void CMDHelp(Jurnal *J);
void CMDNew(Jurnal *J);
void CMDQuit(Jurnal *J);

#endif