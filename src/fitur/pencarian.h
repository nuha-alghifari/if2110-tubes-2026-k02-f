#ifndef PENCARIAN_H
#define PENCARIAN_H
#include <stdbool.h>
#include "kata.h"
#include "jurnal.h"

typedef struct {
    int id;            
    int idParent;       // 0 jika parent nya Workspace
    Kata judul;
} InfoHalaman;

typedef enum {
    UrutID,
    UrutJudul
} KriteriaUrut;

typedef enum {
    UrutNaik,
    UrutTurun
} ArahUrut;


int KumpulkanHalaman(const Jurnal *J, InfoHalaman **daftar);
int CariHalaman(const InfoHalaman *daftar, int n, const Kata *kunci, InfoHalaman *hasil);
void UrutkanHalaman(InfoHalaman *daftar, int n, KriteriaUrut kriteria, ArahUrut arah);
bool KataKeKriteria(const Kata *k, KriteriaUrut *kriteria);  
bool KataKeArah(const Kata *k, ArahUrut *arah);             
void CetakInfoHalaman(const InfoHalaman *h);                
void CMDSearch(Jurnal *J);
void CMDPages(Jurnal *J);
void CMDSearchPage(Jurnal *J);
void CMDSearchTag(Jurnal *J);

#endif
