# Tokenizer Program C++

## Deskripsi Program

Program ini adalah aplikasi **tokenizer sederhana** yang dibuat menggunakan bahasa pemrograman **C++**. Program dapat membaca input berupa source code program lain, kemudian memecah isi program tersebut menjadi token-token atau string-string bermakna.

Token yang ditemukan akan dikelompokkan ke dalam beberapa kategori, yaitu:

1. **Reserve words**
2. **Simbol dan tanda baca**
3. **Variabel / identifier**
4. **Literal angka**
5. **Literal string**
6. **Literal karakter**
7. **Komentar**
8. **Token tidak dikenal**
9. **Kalimat matematika**, seperti assignment, persamaan, fungsi, ekspresi aritmetika, dan perbandingan

Program ini dirancang sebagai simulasi sederhana dari tahap **lexical analysis** pada compiler.

---

## Tujuan Program

Tujuan utama program ini adalah:

- Membaca source code sebagai input.
- Memisahkan source code menjadi token-token.
- Mengelompokkan token berdasarkan sifatnya.
- Menampilkan posisi token berdasarkan baris dan kolom.
- Mendeteksi kalimat matematika seperti:
  - `a = b + c;`
  - `x += 10;`
  - `sqrt(a*a + b*b);`
  - `if (x >= y)`
- Menyediakan user interface berbasis menu agar pengguna mudah menginputkan program.

---

## Bahasa dan Standar yang Digunakan

Program dibuat menggunakan:

- Bahasa: **C++**
- Standar: **C++17**

Program dapat dikompilasi menggunakan compiler seperti:

- `g++`
- `clang++`
- MinGW untuk Windows

---

## Cara Kompilasi

Gunakan perintah berikut:

```bash
g++ -std=c++17 main.cpp -o main
```

Keterangan:

- `g++` adalah compiler C++.
- `-std=c++17` digunakan agar compiler memakai standar C++17.
- `main.cpp` adalah nama file source code.
- `-o main` menentukan nama file hasil kompilasi.

---

## Cara Menjalankan Program

Setelah program berhasil dikompilasi, jalankan dengan perintah:

```bash
./main
```

Untuk Windows, jika menggunakan MinGW:

```bash
main.exe
```

---

## Menu Program

Saat program dijalankan, pengguna akan melihat menu berikut:

```text
========== TOKENIZER PROGRAM C++ ==========
1. Input program secara manual
2. Baca program dari file
3. Tampilkan source program saat ini
4. Analisis token
5. Simpan hasil analisis ke file
0. Keluar
Pilih menu:
```

Penjelasan menu:

| Menu | Fungsi |
|---|---|
| `1` | Memasukkan source code secara manual melalui terminal |
| `2` | Membaca source code dari file |
| `3` | Menampilkan source code yang sedang tersimpan di memori |
| `4` | Melakukan analisis token dan menampilkan hasilnya |
| `5` | Menyimpan hasil analisis token ke file `.txt` |
| `0` | Keluar dari program |

---

## Cara Input Manual

Jika memilih menu `1`, pengguna dapat mengetik source code secara langsung.

Untuk mengakhiri input, ketik:

```text
###END###
```

Contoh:

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 5;
    int b = 10;
    int c = a + b;

    cout << c << endl;

    return 0;
}
###END###
```

---

## Contoh Source Code yang Dapat Dianalisis

```cpp
#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int a = 5, b = 12;
    double c = sqrt(a*a + b*b);

    if (c >= 10) {
        cout << c << endl;
    }

    return 0;
}
```

Dari source code tersebut, program dapat mendeteksi token seperti:

| Token | Kelompok |
|---|---|
| `include` | Reserve Word |
| `<` | Simbol/Tanda Baca |
| `iostream` | Variabel |
| `int` | Reserve Word |
| `main` | Variabel |
| `(` | Simbol/Tanda Baca |
| `a` | Variabel |
| `=` | Simbol/Tanda Baca |
| `5` | Literal Angka |
| `sqrt` | Variabel / fungsi |
| `+` | Simbol/Tanda Baca |
| `return` | Reserve Word |

Program juga dapat mendeteksi kalimat matematika seperti:

```cpp
int a = 5, b = 12;
double c = sqrt(a*a + b*b);
if (c >= 10)
return 0;
```

---

## Struktur Data Utama

Program menggunakan dua struktur data utama, yaitu `Token` dan `MathSentence`.

### 1. Struct `Token`

```cpp
struct Token {
    string lexeme;
    string category;
    int line;
    int col;
};
```

Struct ini digunakan untuk menyimpan informasi setiap token.

Penjelasan field:

| Field | Fungsi |
|---|---|
| `lexeme` | Menyimpan teks asli token, misalnya `int`, `main`, `=`, `5` |
| `category` | Menyimpan kategori token |
| `line` | Menyimpan nomor baris kemunculan token |
| `col` | Menyimpan nomor kolom kemunculan token |

Contoh isi `Token`:

```cpp
{ "int", "Reserve Word", 4, 1 }
```

Artinya token `int` termasuk reserve word dan ditemukan pada baris 4 kolom 1.

---

### 2. Struct `MathSentence`

```cpp
struct MathSentence {
    string text;
    string reason;
    int line;
};
```

Struct ini digunakan untuk menyimpan informasi kalimat matematika yang ditemukan.

Penjelasan field:

| Field | Fungsi |
|---|---|
| `text` | Menyimpan teks kalimat matematika |
| `reason` | Menyimpan alasan mengapa kalimat tersebut dianggap kalimat matematika |
| `line` | Menyimpan nomor baris awal kalimat matematika |

Contoh isi `MathSentence`:

```cpp
{ "double c = sqrt(a * a + b * b);", "persamaan/assignment, ekspresi aritmetika, fungsi matematika", 6 }
```

---

## Konstanta Kategori Token

Program menggunakan beberapa konstanta string untuk kategori token:

```cpp
const string CAT_RESERVED = "Reserve Word";
const string CAT_SYMBOL   = "Simbol/Tanda Baca";
const string CAT_VARIABLE = "Variabel";
const string CAT_NUMBER   = "Literal Angka";
const string CAT_STRING   = "Literal String";
const string CAT_CHAR     = "Literal Karakter";
const string CAT_COMMENT  = "Komentar";
const string CAT_UNKNOWN  = "Tidak Dikenal";
```

Konstanta ini digunakan agar penamaan kategori token konsisten di seluruh bagian program.

---

# Penjelasan Fungsi Program

## 1. Fungsi `isIdentifierStart`

```cpp
bool isIdentifierStart(char c)
```

Fungsi ini digunakan untuk memeriksa apakah suatu karakter dapat menjadi karakter pertama dari sebuah identifier atau variabel.

Identifier dalam C++ boleh diawali oleh:

- Huruf, misalnya `a`, `B`, `x`
- Underscore `_`

Identifier tidak boleh diawali oleh angka.

Contoh:

| Karakter | Hasil |
|---|---|
| `a` | true |
| `Z` | true |
| `_` | true |
| `5` | false |
| `+` | false |

Fungsi ini penting karena tokenizer harus bisa membedakan antara variabel, reserve word, angka, dan simbol.

---

## 2. Fungsi `isIdentifierPart`

```cpp
bool isIdentifierPart(char c)
```

Fungsi ini digunakan untuk memeriksa apakah suatu karakter boleh menjadi bagian dari identifier setelah karakter pertama.

Karakter yang diperbolehkan:

- Huruf
- Angka
- Underscore `_`

Contoh identifier valid:

```cpp
nama
nilai1
_total
angka_ke_2
```

Contoh proses:

Jika input adalah:

```cpp
nilai1 = 10;
```

Tokenizer akan membaca:

```text
n i l a i 1
```

sebagai satu token identifier karena semua karakter tersebut valid sebagai bagian dari identifier.

---

## 3. Fungsi `advancePosition`

```cpp
void advancePosition(char c, int &line, int &col)
```

Fungsi ini digunakan untuk memperbarui posisi baris dan kolom ketika tokenizer membaca satu karakter.

Jika karakter yang dibaca adalah newline:

```cpp
'\n'
```

maka:

- Nomor baris bertambah satu.
- Nomor kolom kembali menjadi `1`.

Jika karakter bukan newline, maka kolom bertambah satu.

Contoh:

```text
int a;
```

Saat membaca `i`, posisi kolom bertambah. Saat membaca newline, baris bertambah.

Fungsi ini berguna agar setiap token dapat ditampilkan bersama posisi kemunculannya.

---

## 4. Fungsi `displayLexeme`

```cpp
string displayLexeme(const string &s, size_t limit = 45)
```

Fungsi ini digunakan untuk menampilkan token dengan bentuk yang lebih aman dan rapi di output.

Fungsi ini mengubah karakter khusus menjadi bentuk teks:

| Karakter Asli | Ditampilkan Sebagai |
|---|---|
| newline | `\n` |
| tab | `\t` |
| carriage return | `\r` |

Fungsi ini juga membatasi panjang teks token agar tabel output tidak rusak.

Contoh:

Jika token berisi string panjang:

```cpp
"Ini adalah string yang sangat panjang sekali..."
```

maka output dapat dipotong menjadi:

```text
"Ini adalah string yang sangat panjang seka...
```

Tujuannya adalah menjaga tampilan laporan tetap rapi.

---

## 5. Fungsi `buildReservedWords`

```cpp
unordered_set<string> buildReservedWords()
```

Fungsi ini membuat kumpulan reserve words atau kata kunci bahasa C++.

Contoh reserve words:

```cpp
int
float
double
if
else
while
for
return
class
struct
namespace
using
```

Fungsi ini mengembalikan struktur data `unordered_set<string>`.

Alasan menggunakan `unordered_set` adalah agar pengecekan kata kunci dapat dilakukan dengan cepat.

Contoh penggunaan:

```cpp
if (RESERVED_WORDS.count(lexeme)) {
    addToken(lexeme, CAT_RESERVED, startLine, startCol);
}
```

Artinya, jika lexeme ditemukan di daftar reserve words, token akan dikategorikan sebagai `Reserve Word`.

---

## 6. Fungsi `isTypeWord`

```cpp
bool isTypeWord(const string &s)
```

Fungsi ini memeriksa apakah sebuah string termasuk kata yang biasa digunakan sebagai tipe data.

Contoh type word:

```cpp
int
float
double
char
bool
void
long
short
string
auto
```

Fungsi ini digunakan terutama saat mendeteksi kalimat matematika.

Contoh:

```cpp
int main()
```

Program tidak langsung menganggap `main()` sebagai pemanggilan fungsi matematika karena sebelumnya terdapat tipe data `int`.

Dengan fungsi ini, program dapat membedakan antara:

```cpp
int main()
```

dan:

```cpp
sqrt(x)
```

---

## 7. Fungsi `buildMultiCharOperators`

```cpp
vector<string> buildMultiCharOperators()
```

Fungsi ini membuat daftar operator yang memiliki lebih dari satu karakter.

Contoh operator multi-karakter:

```cpp
==
!=
<=
>=
++
--
+=
-=
*=
/=
%=
&&
||
<<
>>
::
->
```

Fungsi ini juga mengurutkan operator dari yang paling panjang ke yang paling pendek.

Hal ini penting untuk menerapkan prinsip **longest match**.

Contoh:

Jika input adalah:

```cpp
a >>= 2;
```

Tokenizer harus membaca `>>=` sebagai satu token, bukan:

```text
>
>
=
```

Dengan mengurutkan operator dari yang paling panjang, tokenizer akan mencoba mencocokkan operator terpanjang terlebih dahulu.

---

## 8. Fungsi `isSingleSymbol`

```cpp
bool isSingleSymbol(char c)
```

Fungsi ini memeriksa apakah suatu karakter termasuk simbol atau tanda baca satu karakter.

Contoh simbol satu karakter:

```cpp
{
}
(
)
[
]
;
,
.
+
-
*
/
%
=
<
>
!
&
|
^
#
```

Jika karakter termasuk daftar simbol tersebut, maka tokenizer akan mengelompokkannya sebagai:

```text
Simbol/Tanda Baca
```

Contoh:

```cpp
int a = 5;
```

Simbol yang ditemukan:

```text
=
;
```

---

## 9. Fungsi `tokenize`

```cpp
vector<Token> tokenize(const string &source)
```

Fungsi ini adalah fungsi utama dalam proses tokenisasi.

Fungsi ini menerima input berupa source code dalam bentuk string, lalu menghasilkan daftar token dalam bentuk:

```cpp
vector<Token>
```

Fungsi ini bekerja dengan membaca source code karakter demi karakter.

Urutan pengecekan dalam fungsi `tokenize` adalah:

1. Mengabaikan whitespace.
2. Mendeteksi komentar satu baris `//`.
3. Mendeteksi komentar blok `/* ... */`.
4. Mendeteksi string literal `"..."`.
5. Mendeteksi character literal `'a'`.
6. Mendeteksi angka.
7. Mendeteksi identifier atau reserve word.
8. Mendeteksi operator multi-karakter.
9. Mendeteksi simbol satu karakter.
10. Menandai karakter tidak dikenal.

Contoh input:

```cpp
int x = 10;
```

Output token:

| Token | Kategori |
|---|---|
| `int` | Reserve Word |
| `x` | Variabel |
| `=` | Simbol/Tanda Baca |
| `10` | Literal Angka |
| `;` | Simbol/Tanda Baca |

Fungsi ini juga mencatat posisi baris dan kolom setiap token.

---

## 10. Fungsi `joinTokenLexemes`

```cpp
string joinTokenLexemes(const vector<Token> &tokens)
```

Fungsi ini digunakan untuk menyusun kembali kumpulan token menjadi bentuk kalimat source code yang mudah dibaca.

Fungsi ini digunakan ketika program ingin menampilkan kalimat matematika.

Contoh token:

```text
double
c
=
sqrt
(
a
*
a
+
b
*
b
)
;
```

Akan disusun menjadi:

```cpp
double c = sqrt(a * a + b * b);
```

Fungsi ini juga mengatur spasi agar hasil output tidak terlihat berantakan.

Contoh:

- Tidak menambahkan spasi sebelum `)`
- Tidak menambahkan spasi setelah `(`
- Tidak menambahkan spasi sebelum `;`

---

## 11. Fungsi `statementStartsWithPreprocessor`

```cpp
bool statementStartsWithPreprocessor(const vector<Token> &st)
```

Fungsi ini memeriksa apakah suatu statement diawali dengan simbol `#`.

Statement yang diawali `#` biasanya merupakan preprocessor directive.

Contoh:

```cpp
#include <iostream>
#define PI 3.14
```

Statement seperti ini tidak dianggap sebagai kalimat matematika biasa.

Fungsi ini digunakan dalam proses klasifikasi kalimat matematika agar baris seperti `#include <iostream>` tidak salah terdeteksi sebagai ekspresi perbandingan karena memiliki simbol `<` dan `>`.

---

## 12. Fungsi `looksLikeSimpleDeclaration`

```cpp
bool looksLikeSimpleDeclaration(const vector<Token> &st)
```

Fungsi ini memeriksa apakah suatu statement terlihat seperti deklarasi variabel sederhana.

Contoh deklarasi sederhana:

```cpp
int a;
double nilai;
char huruf;
```

Deklarasi seperti ini tidak dianggap sebagai kalimat matematika karena tidak memiliki assignment atau operasi matematika.

Namun, statement seperti ini:

```cpp
int a = 10;
```

dapat dianggap sebagai kalimat matematika karena memiliki operator assignment `=`.

Fungsi ini membantu agar program tidak terlalu banyak menghasilkan false positive saat mencari kalimat matematika.

---

## 13. Fungsi `isFunctionCallAt`

```cpp
bool isFunctionCallAt(const vector<Token> &st, size_t i)
```

Fungsi ini memeriksa apakah token pada posisi tertentu merupakan pemanggilan fungsi.

Pola yang dicari adalah:

```cpp
namaFungsi(...)
```

Secara token:

```text
Variabel diikuti oleh "("
```

Contoh yang dianggap function call:

```cpp
sqrt(x)
pow(a, b)
max(x, y)
```

Namun, fungsi ini menghindari menganggap deklarasi fungsi sebagai pemanggilan fungsi.

Contoh:

```cpp
int main()
```

Karena sebelum `main` terdapat type word `int`, maka `main()` tidak dianggap sebagai pemanggilan fungsi biasa.

---

## 14. Fungsi `classifyMathStatement`

```cpp
string classifyMathStatement(const vector<Token> &st)
```

Fungsi ini digunakan untuk menentukan apakah suatu statement termasuk kalimat matematika.

Fungsi ini akan mencari beberapa ciri berikut:

### Assignment

Contoh operator:

```cpp
=
+=
-=
*=
/=
%=
```

Contoh statement:

```cpp
x = 10;
total += nilai;
```

### Operasi aritmetika

Contoh operator:

```cpp
+
-
*
/
%
++
--
```

Contoh statement:

```cpp
c = a + b;
x++;
nilai = total / jumlah;
```

### Operasi perbandingan atau logika

Contoh operator:

```cpp
==
!=
<
>
<=
>=
&&
||
```

Contoh statement:

```cpp
if (x >= y)
while (a != b)
```

### Pemanggilan fungsi

Contoh:

```cpp
sqrt(x)
pow(a, b)
max(x, y)
```

### Fungsi matematika khusus

Program memiliki daftar fungsi matematika umum, seperti:

```cpp
sin
cos
tan
sqrt
pow
log
abs
ceil
floor
round
min
max
gcd
lcm
```

Jika statement memiliki salah satu ciri tersebut, fungsi ini akan mengembalikan alasan klasifikasi.

Contoh hasil:

```text
persamaan/assignment, ekspresi aritmetika, fungsi matematika
```

Jika statement bukan kalimat matematika, fungsi akan mengembalikan string kosong.

---

## 15. Fungsi `extractMathSentences`

```cpp
vector<MathSentence> extractMathSentences(const vector<Token> &tokens)
```

Fungsi ini digunakan untuk mencari semua kalimat matematika dari daftar token.

Cara kerja fungsi:

1. Membaca token satu per satu.
2. Mengabaikan komentar.
3. Mengumpulkan token menjadi statement.
4. Statement dianggap selesai jika menemukan:
   - `;`
   - `{`
   - `}`
5. Statement yang sudah terkumpul dikirim ke fungsi `classifyMathStatement`.
6. Jika statement terdeteksi sebagai kalimat matematika, maka disimpan ke dalam `vector<MathSentence>`.

Contoh:

Input token berasal dari source code:

```cpp
int a = 5;
int b = 10;
int c = a + b;
cout << c << endl;
```

Kalimat matematika yang dapat ditemukan:

```cpp
int a = 5;
int b = 10;
int c = a + b;
```

---

## 16. Fungsi `printSetLine`

```cpp
void printSetLine(ostream &out, const string &title, const set<string> &values)
```

Fungsi ini digunakan untuk mencetak ringkasan token unik berdasarkan kelompok tertentu.

Contoh output:

```text
Reserve words (4):
  int               return            using             namespace
```

Parameter fungsi:

| Parameter | Fungsi |
|---|---|
| `out` | Tujuan output, bisa `cout` atau file |
| `title` | Judul kelompok token |
| `values` | Kumpulan token unik yang akan ditampilkan |

Fungsi ini menggunakan `set<string>` agar token yang sama tidak ditampilkan berulang.

Contoh:

Jika source code memiliki banyak token `int`, ringkasan tetap hanya menampilkan `int` satu kali.

---

## 17. Fungsi `printReport`

```cpp
void printReport(
    ostream &out,
    const vector<Token> &tokens,
    const vector<MathSentence> &mathSentences
)
```

Fungsi ini digunakan untuk mencetak laporan lengkap hasil analisis.

Laporan yang dicetak terdiri dari:

1. Tabel semua token.
2. Ringkasan token berdasarkan kelompok.
3. Daftar kalimat matematika.

Fungsi ini menerima parameter `ostream &out`, sehingga hasil laporan dapat dicetak ke:

- Terminal menggunakan `cout`
- File menggunakan `ofstream`

Contoh penggunaan untuk terminal:

```cpp
printReport(cout, lastTokens, lastMath);
```

Contoh penggunaan untuk file:

```cpp
ofstream fout("hasil_token.txt");
printReport(fout, lastTokens, lastMath);
```

Dengan desain ini, fungsi menjadi lebih fleksibel karena tidak hanya bergantung pada output terminal.

---

## 18. Fungsi `readFileToString`

```cpp
string readFileToString(const string &path)
```

Fungsi ini digunakan untuk membaca seluruh isi file source code menjadi satu string.

Parameter:

| Parameter | Fungsi |
|---|---|
| `path` | Lokasi atau nama file yang ingin dibaca |

Contoh:

```cpp
string content = readFileToString("contoh.cpp");
```

Jika file berhasil dibaca, fungsi akan mengembalikan isi file.

Jika file gagal dibaca, fungsi akan mengembalikan string kosong.

Fungsi ini digunakan pada menu `2`, yaitu menu untuk membaca program dari file.

---

## 19. Fungsi `printMenu`

```cpp
void printMenu()
```

Fungsi ini digunakan untuk menampilkan user interface berbasis menu.

Isi menu:

```text
1. Input program secara manual
2. Baca program dari file
3. Tampilkan source program saat ini
4. Analisis token
5. Simpan hasil analisis ke file
0. Keluar
```

Fungsi ini memisahkan tampilan menu dari fungsi `main`, sehingga kode program menjadi lebih rapi dan mudah dipahami.

---

## 20. Fungsi `main`

```cpp
int main()
```

Fungsi `main` adalah titik awal eksekusi program.

Fungsi ini menjalankan menu utama secara berulang sampai pengguna memilih menu `0`.

Variabel penting di dalam `main`:

```cpp
string sourceCode;
vector<Token> lastTokens;
vector<MathSentence> lastMath;
bool alreadyAnalyzed;
```

Penjelasan:

| Variabel | Fungsi |
|---|---|
| `sourceCode` | Menyimpan source code yang sedang dianalisis |
| `lastTokens` | Menyimpan hasil tokenisasi terakhir |
| `lastMath` | Menyimpan hasil deteksi kalimat matematika terakhir |
| `alreadyAnalyzed` | Menandai apakah source code sudah dianalisis atau belum |

Alur kerja fungsi `main`:

1. Menampilkan menu.
2. Membaca pilihan pengguna.
3. Menjalankan aksi berdasarkan pilihan.
4. Mengulang proses sampai pengguna memilih keluar.

Contoh alur:

```text
Input source code
        ↓
Tokenisasi
        ↓
Cetak laporan
        ↓
Simpan hasil ke file jika diperlukan
```

---

# Alur Algoritma Tokenisasi

Secara umum, algoritma tokenisasi bekerja sebagai berikut:

```text
Mulai
  ↓
Baca source code
  ↓
Baca karakter satu per satu
  ↓
Jika whitespace, lewati
  ↓
Jika komentar, baca sampai komentar selesai
  ↓
Jika string, baca sampai tanda petik penutup
  ↓
Jika angka, baca seluruh angka
  ↓
Jika identifier, cek apakah reserve word atau variabel
  ↓
Jika operator multi-karakter, gunakan longest match
  ↓
Jika simbol satu karakter, simpan sebagai simbol
  ↓
Jika tidak dikenali, simpan sebagai token tidak dikenal
  ↓
Tampilkan hasil
Selesai
```

---

# Alur Deteksi Kalimat Matematika

Deteksi kalimat matematika dilakukan setelah proses tokenisasi selesai.

Alurnya:

```text
Daftar token
  ↓
Gabungkan token menjadi statement
  ↓
Abaikan komentar
  ↓
Cek apakah statement mengandung assignment
  ↓
Cek apakah statement mengandung operasi aritmetika
  ↓
Cek apakah statement mengandung perbandingan/logika
  ↓
Cek apakah statement mengandung function call
  ↓
Jika memenuhi salah satu ciri, simpan sebagai kalimat matematika
```

Contoh statement:

```cpp
double c = sqrt(a*a + b*b);
```

Ciri yang ditemukan:

- Ada assignment `=`
- Ada operator aritmetika `*` dan `+`
- Ada fungsi matematika `sqrt`

Maka statement tersebut dikategorikan sebagai kalimat matematika.

---

# Kelebihan Program

Program ini memiliki beberapa kelebihan:

1. **Mendukung input manual dan input dari file**

   Pengguna dapat mengetik source code langsung atau membaca file `.cpp`.

2. **Menampilkan posisi token**

   Setiap token memiliki informasi baris dan kolom sehingga lebih mudah dilacak.

3. **Mendeteksi operator multi-karakter**

   Program dapat membaca operator seperti `>=`, `==`, `+=`, `++`, `::`, dan `->`.

4. **Mendeteksi komentar**

   Program dapat mengenali komentar satu baris dan komentar blok.

5. **Mendeteksi literal**

   Program dapat mengenali literal angka, string, dan karakter.

6. **Mendeteksi kalimat matematika**

   Program dapat mengenali assignment, ekspresi aritmetika, fungsi matematika, dan perbandingan.

7. **Dapat menyimpan hasil ke file**

   Hasil analisis dapat disimpan ke file `.txt`.

---

# Batasan Program

Program ini adalah tokenizer sederhana, bukan compiler C++ penuh.

Beberapa batasannya adalah:

1. Program belum melakukan parsing sintaks secara lengkap.
2. Program belum memvalidasi apakah source code benar secara grammar C++.
3. Deteksi kalimat matematika masih berbasis pola atau heuristik.
4. Program belum membedakan secara penuh antara nama fungsi, nama variabel, nama class, dan namespace.
5. Program belum menangani semua bentuk literal C++ modern secara sempurna.
6. Program belum melakukan semantic analysis.

Walaupun demikian, program sudah cukup untuk menunjukkan konsep dasar lexical analysis dan pengelompokan token.

---

# Contoh Output Ringkas

Contoh source code:

```cpp
int a = 5;
int b = 10;
int c = a + b;
```

Contoh hasil token:

| No | Token | Kelompok | Baris | Kolom |
|---|---|---|---|---|
| 1 | `int` | Reserve Word | 1 | 1 |
| 2 | `a` | Variabel | 1 | 5 |
| 3 | `=` | Simbol/Tanda Baca | 1 | 7 |
| 4 | `5` | Literal Angka | 1 | 9 |
| 5 | `;` | Simbol/Tanda Baca | 1 | 10 |
| 6 | `int` | Reserve Word | 2 | 1 |
| 7 | `b` | Variabel | 2 | 5 |
| 8 | `=` | Simbol/Tanda Baca | 2 | 7 |
| 9 | `10` | Literal Angka | 2 | 9 |
| 10 | `;` | Simbol/Tanda Baca | 2 | 11 |
| 11 | `int` | Reserve Word | 3 | 1 |
| 12 | `c` | Variabel | 3 | 5 |
| 13 | `=` | Simbol/Tanda Baca | 3 | 7 |
| 14 | `a` | Variabel | 3 | 9 |
| 15 | `+` | Simbol/Tanda Baca | 3 | 11 |
| 16 | `b` | Variabel | 3 | 13 |
| 17 | `;` | Simbol/Tanda Baca | 3 | 14 |

Kalimat matematika yang ditemukan:

```text
1. Baris 1 | persamaan/assignment
   int a = 5;

2. Baris 2 | persamaan/assignment
   int b = 10;

3. Baris 3 | persamaan/assignment, ekspresi aritmetika
   int c = a + b;
```

---

# Kesimpulan

Program ini merupakan implementasi sederhana dari proses tokenisasi pada compiler. Program membaca source code, memecahnya menjadi token, lalu mengelompokkan token berdasarkan jenisnya.

Dengan adanya user interface berbasis menu, pengguna dapat lebih mudah memasukkan source code, menjalankan analisis, melihat hasil, dan menyimpan laporan ke file.

Program ini juga memiliki fitur tambahan berupa deteksi kalimat matematika, sehingga tidak hanya menampilkan token satu per satu, tetapi juga mencoba mengenali statement yang mengandung operasi matematika, assignment, fungsi, dan perbandingan.
