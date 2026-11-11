#include "inisialisasi.h"
#include "jurnal.h"
#include "perintah.h"

int main(void){
    Jurnal jurnal;

    InitJurnal(&jurnal);
    if (Inisialisasi(&jurnal)){
        JalankanPerintah(&jurnal);
    }
    KosongkanJurnal(&jurnal);
    return 0;
}