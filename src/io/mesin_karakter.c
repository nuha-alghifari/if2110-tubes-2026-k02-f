#include "mesin_karakter.h"

static void Ambil (MesinKarakter *m){
    if (m->file != NULL){
        int c = fgetc(m->file);
        if (c == EOF){
            m->cc = '\0';
            m->eop = true;
        } else{
            m->cc = (char) c;
            m->eop = false;
        }
    } else if (m->str != NULL && m->str[m->idx] != '\0'){
        m->cc = m->str[m->idx];
        m->idx++;
        m->eop = false;
    } else {
        m->cc = '\0';
        m->eop = true;
    }
}

void StartFile(MesinKarakter *m, FILE *f){
    m->file = f;
    m->str = NULL;
    m->idx = 0;
    Ambil (m);
}

void StartString(MesinKarakter *m, const char *s){
    m->file = NULL;
    m->str = s;
    m->idx = 0;
    Ambil (m);
}

void Adv(MesinKarakter *m){
    if (m->eop){
        return;
    }
    Ambil(m);
}