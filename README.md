# Tugas Besar IF2110 - NangorNotes (K02-F)

**NangorNotes** adalah sebuah aplikasi jurnal digital berbasis terminal yang terinspirasi dari aplikasi Notion dan ditulis dalam bahasa pemrograman C. Pengguna dapat menyusun halaman dan subhalaman, mengisi blok teks dan checklist, menghubungkan halaman dengan link/backlink, memberi tag, mencari dan mengurutkan halaman, serta menyimpan dan memuat jurnal.

##  Anggota Kelompok
| Nama | NIM |
| ---- | --- |
| *Daniel Putra Rywandi S* | *13524143* |
| *Klio Lysander* | *13525020* |
| *Mochammad Nuha Al Ghifari* | *13525056* |
| *Kelvin Sebastian Yen* | *13525068* |
| *Kevin Lincoln Hutabarat* | *13525101* |

## Cara Kompilasi dan Menjalankan

| Perintah                                      | Kegunaan                                          |
| --------------------------------------------- | ------------------------------------------------- |
| `make`                                        | Mengompilasi seluruh `.c` di `src/` ke `bin/nangornotes`. |
| `make run`                                    | Mengompilasi jika perlu, lalu menjalankan aplikasi. |
| `make test`                                   | Mengompilasi dan menjalankan semua driver di `drivers/`. |
| `make run-driver DRIVER=drivers/driverskata.c` | Mengompilasi dan menjalankan satu driver.         |
| `make clean`                                  | Menghapus `build/` dan `bin/`; sample data tetap ada. |
| `make help`                                   | Menampilkan bantuan target.                       |

Saat program dijalankan, akan diminta sebuah folder konfigurasi (dapat menggunakan 'sample-data/jurnal' atau 'sample-data/kosong').

Command 'HELP;' dapat digunakan untuk melihat daftar seluruh perintah.

## Cara Menjalankan Driver ADT

Setiap driver berada pada folder 'drivers/' yang masing masing programnya sudah memiliki main sendiri.

```sh
make test                                         
make run-driver DRIVER=drivers/driverskata.c  #contoh 1 driver saja    
```

| Driver | Menguji |
| ------ | ------- |
| `driverskata.c` | ADT Kata dan util string |
| `driversmesin.c` | Mesin karakter, mesin kata, dan pembaca field CSV |
| `driversinput.c` | Pembacaan masukan interaktif |
| `driversjurnal.c` | Struct Jurnal: counter ID, flag perubahan, dan pemuatan data |
| `driversblock.c` | Blok teks dan checklist |
| `driverspencarian.c` | Pencarian dan pengurutan halaman |
| `driverstag.c` | ADT Tag: validasi nama, salin, dan perbandingan |
| `driverslinkedlist.c` | List berkait halaman dan list berkait tag |
| `driversmultilist.c` | Multilist halaman ke tag dan tag ke halaman |
| `driversgraph.c` | Graph berarah untuk link dan backlink |
| `driverstree.c` | Tree halaman |
 
`make test` menjalankan setiap driver dengan masukan kosong sehingga `driversinput` tidak menunggu ketikan. Contoh input untuk mencoba `driversinput` secara manual:
 
```sh
printf 'halo;\nP12;\nabc;\n42;\nx;\ny;\n' | ./bin/drivers/driversinput
```

## Struktur Folder
 
```text
.
├── Makefile
├── README.md
├── src/
│   ├── main.c        
│   ├── adt/          # ADT: Kata, Tree, List Berkait, Graph, Multilist, Block, Tag
│   ├── io/           # Mesin Karakter, Mesin Kata, Mesin CSV
│   └── fitur/        # Command loop, input, inisialisasi, dan perintah fitur
├── drivers/          # Driver pengujian ADT
├── sample-data/      # jurnal, kosong, rusak
└── doc/              # Laporan (PDF)
```

Folder `build/` dan `bin/` dibuat otomatis saat kompilasi dan tidak masuk Git.

## Pembagian Tugas

| Nama | Tugas | 
| ---- | --- |
| *Daniel Putra Rywandi S* | *F05, F08, skeleton Makefile* |
| *Klio Lysander* | *ADT I/O, ADT Kata, command loop, F01, F10, README* |
| *Mochammad Nuha Al Ghifari* | *F09, parser CSV, validasi file rusak, sample-data rusak buat tes* |
| *Kelvin Sebastian Yen* | *F02, F03, F04* |
| *Kevin Lincoln Hutabarat* | *F06, F07* |

## Status Fitur

| Kode | Fitur | Status |
| ---- | ----- | ------ |
| F00 | Rencana implementasi | Selesai |
| F01 | Inisialisasi | Selesai |
| F02 | Manajemen halaman | Selesai |
| F03 | Hierarki halaman | Selesai |
| F04 | Pemindahan dan penghapusan halaman | Selesai |
| F05 | Blok teks dan checklist | Selesai |
| F06 | Link dan backlink | Selesai |
| F07 | Tag | Selesai |
| F08 | Pencarian dan pengurutan halaman | Selesai |
| F09 | Save dan Load | Selesai |
| F10 | Perintah umum (HELP, NEW, QUIT) | Selesai |

## Status Bonus
 
| Kode | Bonus | Status |
| ---- | ----- | ------ |
| B01 | Git Best Practice | Selesai |
| B02 | Keyword Analysis | Tidak |
| B03 | Advanced Search | Tidak |
| B04 | Undo/Redo Perubahan Blok | Tidak |
| B05 | Jalur Referensi Terpendek | Tidak |
| B06 | Data Security | Tidak |
| BXX | Kreativitas | Tidak |

## Sample Data

- **sample-data/jurnal:** 3 halaman, 3 blok, 2 tag, 2 link, dan 3 relasi halaman–tag.
- **sample-data/kosong:** jurnal kosong yang valid.
- **sample-data/rusak:** data dengan referensi parent yang tidak ada, untuk menguji validasi LOAD.
Penjelasan isi dan ID berikutnya ada pada [sample-data/README.md](sample-data/README.md).

