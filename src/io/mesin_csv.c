#include "mesin_csv.h"

static bool AkhirField(const MesinKarakter *m){
    return m->cc == ',' || m->cc == '\n' || m->eop || m->cc == '\r';
}

static PemisahCSV AmbilPemisah(MesinKarakter *m){
    if (m->cc == ','){
        Adv(m);
        return pemisahKoma;
    }
    if (m->cc == '\r'){
        Adv(m);
        if (!m->eop && m-> cc == '\n'){
            Adv(m);
        }
    } else if (m->cc == '\n') Adv(m);
    return pemisahBaris;
}

StatusCSV BacaFieldCSV(MesinKarakter *m, Kata *hasil, bool *berkutip, PemisahCSV *pemisah){
    KosongKata(hasil);
    *berkutip = false;
    *pemisah = pemisahBaris;
    if(m->eop)return CsvEOF;
    if (m->cc == '"'){
        *berkutip = true;
        Adv(m);
        while (true){
            if (m->eop) return CsvSalahFormat;
            if (m->cc == '\n' || m-> cc == '\r') return CsvSalahFormat;
            if (m->cc == '"'){
                Adv(m);
                if (m->eop || m->cc != '"') break;
            }
            if (!TambahKata(hasil, m->cc)) return CsvSalahFormat;
            Adv(m);
        }
        if (!AkhirField(m)) return CsvSalahFormat;
    } else {
        while (!AkhirField(m)){
            if (m->cc == '"') return CsvSalahFormat;
            if (!TambahKata(hasil, m->cc)) return CsvSalahFormat;
            Adv(m);
        }
    }
    *pemisah = AmbilPemisah(m);
    return CsvOK;
} 