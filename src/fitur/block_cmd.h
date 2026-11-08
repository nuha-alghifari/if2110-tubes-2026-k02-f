#ifndef BLOCK_CMD_H
#define BLOCK_CMD_H
#include <stdbool.h>
#include "kata.h"
#include "block.h"

bool ProsesBlockAdd(ListBlok *list, int idBlokBaru, const Kata *tipe, const Kata *posisi, const Kata *teks);
bool ProsesBlockEdit(ListBlok *list, int idBlok, const Kata *teksBaru);
bool ProsesBlockMove(ListBlok *list, int idHalaman, int idBlok, const Kata *posisiBaru);
bool ProsesBlockDelete(ListBlok *list, int idBlok);
bool ProsesCheck(ListBlok *list, int idBlok, const Kata *status);

void CetakDaftarBlok(const ListBlok *list);

void PerintahBlockAdd(void);
void PerintahBlockEdit(void);
void PerintahBlockMove(void);
void PerintahBlockDelete(void);
void PerintahCheck(void);

#endif
