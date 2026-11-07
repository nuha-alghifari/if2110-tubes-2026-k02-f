#include "mesin_kata.h"

StatusBaca BacaMasukan(MesinKarakter *m, Kata *hasil){
    bool kepanjangan = false;
    KosongKata(hasil);
    while (!m->eop && m->cc != ';'){
        char c = m->cc;
        if (c == '\n' || c == '\r' || c == '\t') c = ' ';
        if (!TambahKata(hasil, m->cc)){
            kepanjangan = true;
        }
        Adv(m);
    }
    if (m->eop) {
        KosongKata(hasil);
        return BacaEOF;
    }
    Adv(m);

    if (kepanjangan){
        KosongKata(hasil);
        return BacaTerlaluPanjang;
    }
    PangkasKata(hasil);
    return BacaOK;
}