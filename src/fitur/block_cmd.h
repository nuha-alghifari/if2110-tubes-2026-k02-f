#ifndef BLOCK_CMD_H
#define BLOCK_CMD_H
#include <stdbool.h>
#include "kata.h"
#include "block.h"
#include "jurnal.h"

// validasi, ubah list, cetak hasil. Mengembalikan true jika berhasil.
bool ProsesBlockAdd(ListBlok *list, int idBlokBaru, const Kata *tipe, const Kata *posisi, const Kata *teks);
bool ProsesBlockEdit(ListBlok *list, int idBlok, const Kata *teksBaru);
bool ProsesBlockMove(ListBlok *list, int idHalaman, int idBlok, const Kata *posisiBaru);
bool ProsesBlockDelete(ListBlok *list, int idBlok);
bool ProsesCheck(ListBlok *list, int idBlok, const Kata *status);

void CetakDaftarBlok(const ListBlok *list);   

//  baca input, lalu panggil Proses...
void CMDBlockAdd(Jurnal *J);
void CMDBlockEdit(Jurnal *J);
void CMDBlockMove(Jurnal *J);
void CMDBlockDelete(Jurnal *J);
void CMDCheck(Jurnal *J);

#endif
