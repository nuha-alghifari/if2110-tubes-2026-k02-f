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
| `driversjurnal.c` | Counter ID dan flag perubahan jurnal |
| `driversblock.c` | Blok teks dan checklist |
| `driverspencarian.c` | Pencarian dan pengurutan halaman |
 
`driversinput` bersifat interaktif; jalankan dengan masukan dari pipe, misalnya:
 
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
| *Klio Lysander* | *ADT I/O, ADT Kata + util string, command loop, F01, F10, README* |
| *Mochammad Nuha Al Ghifari* | *F09, parser CSV, validasi file rusak, sample-data rusak buat tes* |
| *Kelvin Sebastian Yen* | *F02, F03, F04* |
| *Kevin Lincoln Hutabarat* | *F06, F07* |

## Status Fitur

| Kode | Fitur | Status |
| ---- | ----- | ------ |
| F00 | Rencana implementasi (ADT, driver, alasan di laporan) | Selesai |
| F01 | Inisialisasi | Belum |
| F02 | Manajemen halaman | Belum |
| F03 | Hierarki halaman | Belum |
| F04 | Pemindahan dan penghapusan halaman | Belum |
| F05 | Blok teks dan checklist | Belum |
| F06 | Link dan backlink | Belum |
| F07 | Tag | Belum |
| F08 | Pencarian dan pengurutan halaman | Belum |
| F09 | Save dan Load | Belum |
| F10 | Perintah umum (HELP, NEW, QUIT) | Selesai |

## Sample Data

- **sample-data/jurnal:** 3 halaman, 3 blok, 2 tag, 2 link, dan 3 relasi halaman–tag.
- **sample-data/kosong:** jurnal kosong yang valid.
- **sample-data/rusak:** data dengan referensi parent yang tidak ada, untuk menguji validasi LOAD.
Penjelasan isi dan ID berikutnya ada pada [sample-data/README.md](sample-data/README.md).

