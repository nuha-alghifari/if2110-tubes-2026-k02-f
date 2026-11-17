#include "jurnal.h"

void InitJurnal(Jurnal *j){
    j->nextBlockId = 1;
    j->nextPageId = 1;
    j->nextTagId = 1;
    j->perubahan = false;
}

void KosongkanJurnal(Jurnal *j){
    InitJurnal(j);
}

void TukarJurnal(Jurnal *a, Jurnal *b){
    Jurnal temp = *a;
    *a = *b;
    *b = temp;
}

int AmbilIDHalaman(Jurnal *j){
    int id = j->nextPageId;
    j->nextPageId++;
    return id;
}

int AmbilIDBlok(Jurnal *j){
    int id = j->nextBlockId;
    j->nextBlockId++;
    return id;
}

int AmbilIDTag(Jurnal *j){
    int id = j->nextTagId;
    j->nextTagId++;
    return id;
}

void TandaiUbah(Jurnal *j){
    j->perubahan = true;
}

void TandaiTersimpan(Jurnal *j){
    j->perubahan = false;
}