# Modul 2 – Pemrograman GPIO
<img width="1366" height="1057" alt="Screenshot 2026-04-13 115908" src="https://github.com/user-attachments/assets/d0bae735-1274-4e2e-a9cd-bbe10416c92a" />

___

## 👤 Data Praktikan
- **Nama:** Iven Rival Pangestu
- **NIM:** H1H024013
- **Shift:** A
___

## 2.5.4 Pertanyaan Praktikum 
1. Gambarkan rangkaian schematic yang digunakan pada percobaan!

   <img width="688" height="429" alt="7segmen-1" src="https://github.com/user-attachments/assets/108cca5c-409c-44ae-9994-726e5339e27e" />

   
3. Apa yang terjadi jika nilai num lebih dari 15? 

Berdasarkan logika program, array digitPattern memiliki indeks maksimal 15 (0-F). Jika nilai num lebih dari 15 tanpa ada pembatasan logika, program akan mencoba mengakses memori di luar batas array (index out of bounds), yang dapat menyebabkan tampilan segmen menjadi tidak menentu atau program mengalami crash. Namun, pada hasil percobaan, sistem dirancang untuk kembali ke 0 (looping) setelah mencapai F.

3. Apakah program ini menggunakan common cathode atau common anode? Jelaskan 
alasanya! 

Program ini menggunakan Common Anode. Alasannya adalah:Dalam fungsi displayDigit(), digunakan operator negasi logika (!) terhadap digitPattern.
Pada tipe Common Anode, segmen akan menyala (aktif) ketika pin output Arduino memberikan sinyal LOW (logika 0)

4. Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap 
baris kode nya dalam bentuk README.md!

<img width="1366" height="1057" alt="image-3" src="https://github.com/user-attachments/assets/365b95f6-f92e-4602-97f0-b7374c4cd6cc" />
# Proyek Seven Segment Hexadecimal Counter (Countdown)

Kode ini digunakan untuk menampilkan angka hexadecimal (0-F) secara hitung mundur (countdown) menggunakan Seven Segment **Common Anode**.

## Skema Pin
| Segmen | Pin Arduino |
|--------|-------------|
| a      | 7           |
| b      | 6           |
| c      | 5           |
| d      | 10          |
| e      | 11          |
| f      | 8           |
| g      | 9           |
| DP     | 4           |

## Kode Program

```cpp
#include <Arduino.h>

// Mapping pin segmen: a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 10, 11, 8, 9, 4};

// Alias nama pin untuk keterbacaan
#define pinA  segmentPins[0]
#define pinB  segmentPins[1]
#define pinC  segmentPins[2]
#define pinD  segmentPins[3]
#define pinE  segmentPins[4]
#define pinF  segmentPins[5]
#define pinG  segmentPins[6]
#define pinDP segmentPins[7]

/**
 * Pola bit untuk karakter heksadesimal (0-F)
 * Urutan: {a, b, c, d, e, f, g}
 * Perangkat: Common Anode (1 = ON/LOW, 0 = OFF/HIGH)
 */
byte hexPatterns[16][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}, // 9
  {1, 1, 1, 0, 1, 1, 1}, // A
  {0, 0, 1, 1, 1, 1, 1}, // b
  {1, 0, 0, 1, 1, 1, 0}, // C
  {0, 1, 1, 1, 1, 0, 1}, // d
  {1, 0, 0, 1, 1, 1, 1}, // E
  {1, 0, 0, 0, 1, 1, 1}  // F
};

void displayHex(int num);

void setup() {
  // Inisialisasi semua pin sebagai OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], HIGH); // Matikan semua (Common Anode)
  }
}

void loop() {
  // Countdown dari F (15) ke 0
  for (int i = 15; i >= 0; i--) {
    displayHex(i);
    delay(1000); 
  }
}

void displayHex(int num) {
  // Menampilkan pola berdasarkan index num
  digitalWrite(pinA, hexPatterns[num][0] ? LOW : HIGH);
  digitalWrite(pinB, hexPatterns[num][1] ? LOW : HIGH);
  digitalWrite(pinC, hexPatterns[num][2] ? LOW : HIGH);
  digitalWrite(pinD, hexPatterns[num][3] ? LOW : HIGH);
  digitalWrite(pinE, hexPatterns[num][4] ? LOW : HIGH);
  digitalWrite(pinF, hexPatterns[num][5] ? LOW : HIGH);
  digitalWrite(pinG, hexPatterns[num][6] ? LOW : HIGH);
  
  digitalWrite(pinDP, HIGH); // DP selalu mati
}

void loop() {
  // *** MODIFIKASI: Loop berjalan dari 15 (F) turun ke 0 ***
  // Menggunakan decrement (i--) agar urutan terbalik dibanding versi asli
  for (int i = 15; i >= 0; i--) {
    // i dimulai dari 15 (F) dan berkurang setiap iterasi hingga 0
    
    displayHex(i);    // Tampilkan karakter ke-i pada Seven Segment Display
    delay(1000);      // Tunda 1000ms (1 detik) sebelum menampilkan karakter berikutnya
  }
  // Setelah i mencapai 0 dan loop selesai, fungsi loop() dipanggil ulang
  // → counter otomatis kembali ke F dan mulai hitung mundur lagi
}

void displayHex(int num) {
  // Fungsi untuk menampilkan satu karakter heksadesimal pada Seven Segment Display
  // Parameter num: indeks karakter (0–15) yang ingin ditampilkan

  // Tulis pola bit ke setiap pin segmen (a sampai g)
  // Operator ternary: jika hexPatterns[num][x] == 1 → tulis LOW (ON)
  //                   jika hexPatterns[num][x] == 0 → tulis HIGH (OFF)
  // Ini karena display bertipe Common Anode: segmen menyala ketika pin LOW
  digitalWrite(pinA, hexPatterns[num][0] ? LOW : HIGH); // Segmen a
  digitalWrite(pinB, hexPatterns[num][1] ? LOW : HIGH); // Segmen b
  digitalWrite(pinC, hexPatterns[num][2] ? LOW : HIGH); // Segmen c
  digitalWrite(pinD, hexPatterns[num][3] ? LOW : HIGH); // Segmen d
  digitalWrite(pinE, hexPatterns[num][4] ? LOW : HIGH); // Segmen e
  digitalWrite(pinF, hexPatterns[num][5] ? LOW : HIGH); // Segmen f
  digitalWrite(pinG, hexPatterns[num][6] ? LOW : HIGH); // Segmen g

  // Segmen dp (titik desimal) selalu dimatikan karena tidak digunakan
  // HIGH = mati pada Common Anode
  digitalWrite(pinDP, HIGH);
}



## 2.6.4 Pertanyaan Praktikum 
1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
   
<img width="1060" height="657" alt="image-1" src="https://github.com/user-attachments/assets/15a0a94e-b960-44dd-b2e2-1ba2fb265d4b" />

2. Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno? 
Apa keuntungannya dibandingkan rangkaian biasa? 

Mode INPUT_PULLUP digunakan untuk mengaktifkan resistor pull-up internal di dalam chip ATmega328P.Keuntungannya: Mengeliminasi gejala sinyal mengambang (floating signal) tanpa memerlukan resistor eksternal tambahan pada breadboard, sehingga rangkaian menjadi lebih sederhana dan efisien.

3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari 
sisi hardware maupun software? 

Sisi Hardware: Kabel jumper putus/longgar, resistor 220 Ohm rusak, LED pada segmen tersebut terbakar, atau kesalahan pemasangan pin pada breadboard.
Sisi Software: Kesalahan pendefinisian nomor pin pada array segmentPins atau kesalahan pola biner (0/1) pada array digitPattern untuk karakter tersebut

4. Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai 
penambahan (increment) dan pengurangan (decrement) pada sistem counter dan 
berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

<img width="1366" height="1057" alt="Screenshot 2026-04-13 115908" src="https://github.com/user-attachments/assets/81a82879-1683-4e75-8816-9040819cc970" />

# Proyek 7-Segment Hexadecimal Up/Down Counter

Proyek ini menggunakan Arduino untuk membuat penghitung (counter) bilangan Hexadecimal (0-F). Angka dapat ditambah atau dikurangi secara manual menggunakan dua tombol push button.

## Fitur Utama
- **Tombol Up**: Menambah angka (0 → F, lalu kembali ke 0).
- **Tombol Down**: Mengurangi angka (F → 0, lalu kembali ke F).
- **Debounce Handling**: Mencegah input ganda saat tombol ditekan.
- **Common Anode Support**: Logika output dibalik (`!`) agar sesuai dengan display Common Anode.

## Skema Pin & Komponen

| Komponen | Pin Arduino | Deskripsi |
| :--- | :---: | :--- |
| **Seven Segment** | 7, 6, 5, 11, 10, 8, 9, 4 | Pin a, b, c, d, e, f, g, dp |
| **Tombol UP** | 2 | Menggunakan Internal Pull-up |
| **Tombol DOWN** | 3 | Menggunakan Internal Pull-up |

## Kode Program (Arduino IDE)

```cpp
#include <Arduino.h>

// Mapping pin segmen: a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Pin untuk tombol navigasi
const int btnUp = 2;   
const int btnDown = 3; 

// Variabel kontrol
int counter = 0;
bool lastUpState = HIGH;
bool lastDownState = HIGH;

/**
 * Matriks pola biner untuk angka 0-F
 * 1 = Segmen Menyala, 0 = Mati
 */
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5 
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}, // 9
  {1,1,1,0,1,1,1,0}, // A
  {0,0,1,1,1,1,1,0}, // b
  {1,0,0,1,1,1,0,0}, // C
  {0,1,1,1,1,0,1,0}, // d
  {1,0,0,1,1,1,1,0}, // E
  {1,0,0,0,1,1,1,0}  // F
};

void displayDigit(int num);

void setup() {
  // Setup Pin Segmen
  for(int i=0; i<8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Setup Tombol dengan Internal

## 2.7 Pertanyaan Praktikum 
1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan! 

Percobaan 2A: Berhasil membuat counter otomatis yang menampilkan karakter heksadesimal 0-F secara berurutan dengan jeda 1 detik.Percobaan 2B: Berhasil mengintegrasikan push button untuk menambah nilai counter secara manual setiap kali tombol ditekan (menggunakan edge detection).

2. Bagaimana prinsip kerja dari Seven Segment Display dalam menampilkan angka dan 
karakter? 

Seven segment bekerja dengan cara mengaktifkan kombinasi spesifik dari 7 LED (a-g). Dengan menyalakan segmen tertentu secara bersamaan (misal segmen a, b, c, d, g untuk angka '3'), mata manusia menangkap bentuk karakter tersebut. Pada tipe Common Anode, semua anoda dihubungkan ke VCC, dan mikrokontroler bertugas memberikan jalur ke Ground (LOW) pada katoda segmen yang ingin dinyalakan

3. Jelaskan bagaimana sistem counter bekerja pada program tersebut! 

Sistem counter bekerja dengan menyimpan sebuah nilai numerik dalam variabel (misal counter).Pada Percobaan 2A, nilai variabel ditambah secara otomatis di dalam loop() dengan bantuan delay(1000).Pada Percobaan 2B, nilai variabel hanya bertambah jika program mendeteksi perubahan status tombol dari HIGH ke LOW (falling edge).
Setiap kali nilai variabel berubah, fungsi displayDigit() dipanggil untuk memperbarui tampilan pada layar sesuai dengan pola bit di digitPattern.
