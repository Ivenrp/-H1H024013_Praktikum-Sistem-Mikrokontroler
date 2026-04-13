# Modul 2 – Pemrograman GPIO
___

Nama: Iven Rival Pangestu
NIM : H1H024013
SHIFT : A
___

## 2.5.4 Pertanyaan Praktikum 
1. Gambarkan rangkaian schematic yang digunakan pada percobaan! 
![alt text](7segmen-1.png)
2. Apa yang terjadi jika nilai num lebih dari 15? 

Berdasarkan logika program, array digitPattern memiliki indeks maksimal 15 (0-F). Jika nilai num lebih dari 15 tanpa ada pembatasan logika, program akan mencoba mengakses memori di luar batas array (index out of bounds), yang dapat menyebabkan tampilan segmen menjadi tidak menentu atau program mengalami crash. Namun, pada hasil percobaan, sistem dirancang untuk kembali ke 0 (looping) setelah mencapai F.

3. Apakah program ini menggunakan common cathode atau common anode? Jelaskan 
alasanya! 

Program ini menggunakan Common Anode. Alasannya adalah:Dalam fungsi displayDigit(), digunakan operator negasi logika (!) terhadap digitPattern.
Pada tipe Common Anode, segmen akan menyala (aktif) ketika pin output Arduino memberikan sinyal LOW (logika 0)

4. Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap 
baris kode nya dalam bentuk README.md!

![alt text](image-3.png)

#include <Arduino.h>
// Mapping pin segmen: a  b  c   d   e  f  g  dp
const int segmentPins[8] = {7, 6, 5, 10, 11, 8, 9, 4};
// Mendefinisikan 8 pin Arduino yang terhubung ke setiap segmen Seven Segment Display.
// Urutan: a=7, b=6, c=5, d=10, e=11, f=8, g=9, dp=4

// Alias nama pin agar kode lebih mudah dibaca
#define pinA  segmentPins[0]   // Pin untuk segmen a (atas)
#define pinB  segmentPins[1]   // Pin untuk segmen b (kanan atas)
#define pinC  segmentPins[2]   // Pin untuk segmen c (kanan bawah)
#define pinD  segmentPins[3]   // Pin untuk segmen d (bawah)
#define pinE  segmentPins[4]   // Pin untuk segmen e (kiri bawah)
#define pinF  segmentPins[5]   // Pin untuk segmen f (kiri atas)
#define pinG  segmentPins[6]   // Pin untuk segmen g (tengah)
#define pinDP segmentPins[7]   // Pin untuk segmen dp (titik)

// Pola bit untuk setiap karakter heksadesimal (urutan: a b c d e f g)
// Nilai 1 = segmen ON, nilai 0 = segmen OFF
// Perangkat: Common Anode → ON = LOW, OFF = HIGH
byte hexPatterns[16][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0 → a,b,c,d,e,f menyala; g mati
  {0, 1, 1, 0, 0, 0, 0}, // 1 → hanya b,c menyala
  {1, 1, 0, 1, 1, 0, 1}, // 2 → a,b,d,e,g menyala
  {1, 1, 1, 1, 0, 0, 1}, // 3 → a,b,c,d,g menyala
  {0, 1, 1, 0, 0, 1, 1}, // 4 → b,c,f,g menyala
  {1, 0, 1, 1, 0, 1, 1}, // 5 → a,c,d,f,g menyala
  {1, 0, 1, 1, 1, 1, 1}, // 6 → a,c,d,e,f,g menyala
  {1, 1, 1, 0, 0, 0, 0}, // 7 → a,b,c menyala
  {1, 1, 1, 1, 1, 1, 1}, // 8 → semua segmen menyala
  {1, 1, 1, 1, 0, 1, 1}, // 9 → a,b,c,d,f,g menyala
  {1, 1, 1, 0, 1, 1, 1}, // A → a,b,c,e,f,g menyala
  {0, 0, 1, 1, 1, 1, 1}, // b → c,d,e,f,g menyala
  {1, 0, 0, 1, 1, 1, 0}, // C → a,d,e,f menyala
  {0, 1, 1, 1, 1, 0, 1}, // d → b,c,d,e,g menyala
  {1, 0, 0, 1, 1, 1, 1}, // E → a,d,e,f,g menyala
  {1, 0, 0, 0, 1, 1, 1}  // F → a,e,f,g menyala
};

// Deklarasi fungsi displayHex sebelum digunakan
void displayHex(int num);

void setup() {
  // Inisialisasi semua pin sebagai OUTPUT
  // Dilakukan satu kali saat Arduino dinyalakan
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    // Gunakan loop agar lebih efisien daripada menulis 8 baris pinMode
  }
  
  // Matikan semua segmen di awal (HIGH = mati pada Common Anode)
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }
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

![alt text](image-1.png)

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

![alt text](image.png)

#include <Arduino.h>
// Mendefinisikan urutan pin untuk segmen: a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Pin untuk tombol penambah dan pemurang
const int btnUp = 2;   
const int btnDown = 3; 

// Variabel penyimpan angka counter (0-15)
int counter = 0;

// Menyimpan status terakhir tombol untuk mendeteksi perubahan (edge detection)
bool lastUpState = HIGH;
bool lastDownState = HIGH;

// Matriks pola biner untuk angka 0-F (1 = Segmen Menyala, 0 = Mati)
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

/**
 * Fungsi untuk mengirim data ke 7-segment
 * Karena menggunakan Common Anode, logika dibalik menggunakan operator '!'
 */
void displayDigit(int num) {
  for(int i=0; i<8; i++) {
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

void setup() {
  // Mengatur semua pin segmen sebagai OUTPUT
  for(int i=0; i<8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Menggunakan INPUT_PULLUP agar tidak memerlukan resistor eksternal pada tombol
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);

  // Menampilkan angka awal (0)
  displayDigit(counter);
}

void loop() {
  // Membaca status tombol saat ini
  bool currentUp = digitalRead(btnUp);
  bool currentDown = digitalRead(btnDown);

  // Logika untuk tombol INCREMENT (Tambah)
  // Mendeteksi transisi dari HIGH ke LOW (tombol ditekan)
  if (lastUpState == HIGH && currentUp == LOW) {
    counter++;
    if(counter > 15) counter = 0; // Reset ke 0 jika lebih dari F
    displayDigit(counter);
    delay(200); // Debounce untuk menghindari pembacaan ganda
  }

  // Logika untuk tombol DECREMENT (Kurang)
  if (lastDownState == HIGH && currentDown == LOW) {
    counter--;
    if(counter < 0) counter = 15; // Lanjut ke F jika kurang dari 0
    displayDigit(counter);
    delay(200); // Debounce
  }

  // Simpan status tombol saat ini untuk iterasi berikutnya
  lastUpState = currentUp;
  lastDownState = currentDown;
}


## 2.7 Pertanyaan Praktikum 
1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan! 

Percobaan 2A: Berhasil membuat counter otomatis yang menampilkan karakter heksadesimal 0-F secara berurutan dengan jeda 1 detik.Percobaan 2B: Berhasil mengintegrasikan push button untuk menambah nilai counter secara manual setiap kali tombol ditekan (menggunakan edge detection).

2. Bagaimana prinsip kerja dari Seven Segment Display dalam menampilkan angka dan 
karakter? 

Seven segment bekerja dengan cara mengaktifkan kombinasi spesifik dari 7 LED (a-g). Dengan menyalakan segmen tertentu secara bersamaan (misal segmen a, b, c, d, g untuk angka '3'), mata manusia menangkap bentuk karakter tersebut. Pada tipe Common Anode, semua anoda dihubungkan ke VCC, dan mikrokontroler bertugas memberikan jalur ke Ground (LOW) pada katoda segmen yang ingin dinyalakan

3. Jelaskan bagaimana sistem counter bekerja pada program tersebut! 

Sistem counter bekerja dengan menyimpan sebuah nilai numerik dalam variabel (misal counter).Pada Percobaan 2A, nilai variabel ditambah secara otomatis di dalam loop() dengan bantuan delay(1000).Pada Percobaan 2B, nilai variabel hanya bertambah jika program mendeteksi perubahan status tombol dari HIGH ke LOW (falling edge).
Setiap kali nilai variabel berubah, fungsi displayDigit() dipanggil untuk memperbarui tampilan pada layar sesuai dengan pola bit di digitPattern.
