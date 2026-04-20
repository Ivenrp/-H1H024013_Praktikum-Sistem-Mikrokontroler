Modul 3 Protokol Komunikasi:

---

## SOAL DAN JAWABAN MODUL 3: PROTOKOL KOMUNIKASI

---

### iv.Pertanyaan Praktikum 

**Soal 1: Jelaskan proses dari input keyboard hingga LED menyala/mati!**

**Jawaban:**
Prosesnya adalah sebagai berikut:
1. Pengguna mengetik karakter ('1' atau '0') di Serial Monitor pada Arduino IDE
2. Karakter tersebut dikirim melalui koneksi USB ke Arduino menggunakan protokol UART
3. Arduino menerima data melalui pin RX
4. Fungsi `Serial.available()` mendeteksi bahwa ada data yang masuk
5. Fungsi `Serial.read()` membaca 1 karakter dari buffer serial
6. Program memeriksa nilai karakter:
   - Jika `'1'` → `digitalWrite(PIN_LED, HIGH)` → LED menyala
   - Jika `'0'` → `digitalWrite(PIN_LED, LOW)` → LED mati
   - Selain itu → Serial Monitor menampilkan "Perintah tidak dikenal"

---

**Soal 2: Mengapa digunakan `Serial.available()` sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?**

**Jawaban:**
`Serial.available()` berfungsi untuk mengecek apakah ada data yang sudah masuk ke buffer serial sebelum mencoba membacanya. Fungsi ini mengembalikan jumlah byte yang tersedia untuk dibaca.

Jika baris tersebut dihilangkan, maka `Serial.read()` akan terus dipanggil setiap iterasi loop meskipun tidak ada data masuk. Akibatnya:
- `Serial.read()` akan mengembalikan nilai `-1` (tidak ada data)
- Kondisi `if (data == '1')` tidak akan pernah terpenuhi secara benar
- Program bisa mengeksekusi blok `else if` yang tidak diinginkan karena nilai `-1` bukan `'\n'` maupun `'\r'`, sehingga "Perintah tidak dikenal" akan terus dicetak berulang kali ke Serial Monitor tanpa henti

---

**Soal 3: Modifikasi program agar LED berkedip (blink) ketika menerima input '2', LED terus berkedip sampai perintah selanjutnya diberikan. Sertakan README.md!**

**Jawaban (Source Code):**

```cpp
#include <Arduino.h>

const int PIN_LED = 8;

// Variabel state untuk mode blink
bool blinkMode = false;

// Variabel untuk milis (non-blocking)
unsigned long previousMillis = 0;
const long interval = 500; // Interval kedip: 500ms
bool ledState = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Ketik '1' = LED ON, '0' = LED OFF, '2' = LED BLINK");
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  // Cek input serial
  if (Serial.available() > 0) {
    char data = Serial.read();

    if (data == '1') {
      blinkMode = false;           // Matikan mode blink
      digitalWrite(PIN_LED, HIGH); // Nyalakan LED
      Serial.println("LED ON");
    }
    else if (data == '0') {
      blinkMode = false;          // Matikan mode blink
      digitalWrite(PIN_LED, LOW); // Matikan LED
      Serial.println("LED OFF");
    }
    else if (data == '2') {
      blinkMode = true;           // Aktifkan mode blink
      Serial.println("LED BLINK");
    }
    else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  // Jalankan blink jika mode aktif (non-blocking dengan millis)
  if (blinkMode) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ledState = !ledState;                   // Toggle state LED
      digitalWrite(PIN_LED, ledState ? HIGH : LOW);
    }
  }
}
```

**README.md:**
```markdown
# Modul 3A - LED Blink dengan UART

## Penjelasan Kode

### Variabel Global
- `blinkMode` : menyimpan status apakah mode blink aktif atau tidak
- `previousMillis` : menyimpan waktu terakhir LED di-toggle
- `interval` : jeda waktu kedip (500ms)
- `ledState` : menyimpan kondisi LED saat ini (ON/OFF)

### setup()
- `Serial.begin(9600)` : inisialisasi komunikasi serial dengan baud rate 9600
- `pinMode(PIN_LED, OUTPUT)` : set Pin 8 sebagai output

### loop()
- `Serial.available() > 0` : mengecek apakah ada data masuk
- `Serial.read()` : membaca 1 karakter dari buffer
- Jika input '1' : blinkMode dimatikan, LED dinyalakan
- Jika input '0' : blinkMode dimatikan, LED dimatikan
- Jika input '2' : blinkMode diaktifkan
- Jika input lain (bukan newline) : tampilkan pesan error

### Bagian Blink (non-blocking)
- Menggunakan `millis()` agar sistem tidak terhenti saat mengedipkan LED
- `currentMillis - previousMillis >= interval` : cek apakah sudah waktunya toggle
- `ledState = !ledState` : membalik kondisi LED setiap interval
```

---

**Soal 4: Tentukan apakah menggunakan `delay()` atau `millis()`! Jelaskan pengaruhnya terhadap sistem.**

**Jawaban:**
Sebaiknya menggunakan **`millis()`**, bukan `delay()`.

Penjelasan perbandingannya:

| Aspek | `delay()` | `millis()` |
|---|---|---|
| Cara kerja | Memblokir eksekusi program selama waktu tertentu | Non-blocking, program tetap berjalan |
| Pembacaan Serial | Tidak bisa membaca input saat delay berlangsung | Bisa membaca input kapan saja |
| Responsivitas | Lambat, sistem "beku" saat delay | Cepat dan responsif |
| Cocok untuk | Program sederhana tanpa input | Program yang perlu multitasking |

Jika menggunakan `delay()`, ketika LED sedang berkedip, Arduino tidak dapat membaca perintah baru dari Serial Monitor karena program tertahan di fungsi `delay()`. Akibatnya, perintah '1' atau '0' untuk menghentikan kedipan tidak akan diproses tepat waktu. Dengan `millis()`, Arduino tetap bisa memeriksa input serial setiap saat sambil menjalankan logika blink.

---

### 3.6.4 Pertanyaan Praktikum

**Soal 1: Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!**

**Jawaban:**
I2C menggunakan dua jalur kabel:
- **SDA (Serial Data)** → terhubung ke pin A4 Arduino: jalur pengiriman data
- **SCL (Serial Clock)** → terhubung ke pin A5 Arduino: jalur sinyal clock sinkronisasi

Cara kerjanya:
1. Arduino (Master) memulai komunikasi dengan mengirimkan **START condition** di jalur SDA
2. Arduino mengirimkan **alamat I2C LCD** (misalnya `0x27`) beserta bit R/W (write)
3. LCD (Slave) yang memiliki alamat `0x27` merespons dengan **ACK (Acknowledge)**
4. Arduino mengirimkan data/perintah (karakter atau instruksi tampilan) byte per byte
5. Setiap byte direspons dengan ACK oleh LCD
6. Setelah selesai, Arduino mengirimkan **STOP condition**
7. Proses ini berulang setiap kali program memanggil `lcd.print()` atau `lcd.setCursor()`

---

**Soal 2: Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!**

**Jawaban:**
Pin potensiometer harus dikonfigurasi dengan benar:
- Kaki kiri → GND (0V)
- Kaki tengah → A0 (wiper/output analog)
- Kaki kanan → 5V (tegangan referensi)

Jika **kaki kiri dan kanan tertukar** (kaki kiri ke 5V, kaki kanan ke GND), maka:
- Arah putaran potensiometer akan **terbalik**
- Saat diputar ke kiri (maksimum), nilai ADC akan membaca **1023** (mendekati 5V)
- Saat diputar ke kanan (minimum), nilai ADC akan membaca **0** (mendekati 0V)
- Fungsi dan tampilan bar di LCD tetap bekerja, tetapi **logika arah putaran berlawanan** dari yang diharapkan
- Secara hardware tidak akan merusak komponen, hanya hasil pembacaan yang terbalik

---

**Soal 3: Modifikasi program gabungan UART + I2C. Sertakan README.md!**

**Jawaban (Source Code):**

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// Inisialisasi LCD I2C alamat 0x27, ukuran 16 kolom x 2 baris
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin potensiometer
const int pinPot = A0;

void setup() {
  Serial.begin(9600); // Inisialisasi UART dengan baud rate 9600
  lcd.init();         // Inisialisasi LCD
  lcd.backlight();    // Nyalakan backlight LCD
}

void loop() {
  // Baca nilai ADC dari potensiometer (0 - 1023)
  int nilai = analogRead(pinPot);

  // Mapping nilai ADC ke panjang bar (0 - 16 karakter)
  int panjangBar = map(nilai, 0, 1023, 0, 16);

  // Hitung tegangan (ADC * 5V / 1023)
  float volt = nilai * (5.0 / 1023.0);

  // Hitung persentase (0 - 100%)
  int persen = map(nilai, 0, 1023, 0, 100);

  // === OUTPUT KE SERIAL MONITOR (UART) ===
  Serial.print("ADC: ");
  Serial.print(nilai);
  Serial.print("  Volt: ");
  Serial.print(volt, 2); // 2 angka desimal
  Serial.print(" V  Persen: ");
  Serial.print(persen);
  Serial.println("%");

  // === OUTPUT KE LCD (I2C) ===
  // Baris 1: Nilai ADC dan persentase
  lcd.setCursor(0, 0);
  lcd.print("ADC:");
  lcd.print(nilai);
  lcd.print(" ");
  lcd.print(persen);
  lcd.print("%   "); // Spasi untuk membersihkan sisa karakter

  // Baris 2: Tampilan bar
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255); // Karakter blok penuh sebagai bar
    } else {
      lcd.print(" "); // Spasi untuk menghapus bar sebelumnya
    }
  }

  delay(200); // Tunggu 200ms sebelum pembacaan berikutnya
}
```

**README.md:**
```markdown
# Modul 3B - Gabungan UART dan I2C

## Penjelasan Kode

### Library yang Digunakan
- `Wire.h` : library komunikasi I2C bawaan Arduino
- `LiquidCrystal_I2C.h` : library untuk mengendalikan LCD via I2C

### Variabel & Objek
- `lcd(0x27, 16, 2)` : objek LCD dengan alamat I2C 0x27, 16 kolom, 2 baris
- `pinPot = A0` : pin analog untuk membaca potensiometer

### setup()
- `Serial.begin(9600)` : memulai komunikasi UART
- `lcd.init()` : menginisialisasi LCD
- `lcd.backlight()` : menyalakan lampu latar LCD

### loop()
- `analogRead(pinPot)` : membaca nilai ADC (0–1023)
- `map(nilai, 0, 1023, 0, 16)` : mengkonversi nilai ADC ke panjang bar
- `nilai * (5.0 / 1023.0)` : mengkonversi ADC ke tegangan (Volt)
- `map(nilai, 0, 1023, 0, 100)` : mengkonversi ADC ke persen

### Output Serial Monitor (UART)
Format: `ADC: [nilai]  Volt: [volt] V  Persen: [persen]%`

### Output LCD (I2C)
- Baris 1 (setCursor(0,0)): Menampilkan nilai ADC dan persentase
- Baris 2 (setCursor(0,1)): Menampilkan bar dari karakter blok (char 255)
  - Karakter 255 = blok penuh (█) sebagai indikator level
  - Spasi untuk menghapus sisa bar dari pembacaan sebelumnya

### delay(200)
Memberikan jeda 200ms agar tampilan tidak berubah terlalu cepat
```

---

**Soal 4: Lengkapi tabel berdasarkan pengamatan Serial Monitor**

**Jawaban (hasil perhitungan teoritis):**

| ADC | Volt (V) | Persen (%) |
|-----|----------|------------|
| 1 | 0.00 V | 0% |
| 21 | 0.10 V | 2% |
| 49 | 0.24 V | 5% |
| 74 | 0.36 V | 7% |
| 96 | 0.47 V | 9% |

> Rumus: Volt = ADC × (5.0 / 1023), Persen = ADC × (100 / 1023)

---

### 3.7 Pertanyaan Praktikum

**Soal 1: Sebutkan dan jelaskan keuntungan dan kerugian menggunakan UART dan I2C!**

**Jawaban:**

**UART:**
| | Keterangan |
|---|---|
| ✅ Keuntungan | Sederhana, tidak butuh sinyal clock eksternal, mudah diimplementasikan, cocok untuk jarak jauh, baud rate fleksibel |
| ❌ Kerugian | Hanya untuk komunikasi point-to-point (2 perangkat), butuh kesesuaian baud rate, tidak ada mekanisme addressing |

**I2C:**
| | Keterangan |
|---|---|
| ✅ Keuntungan | Dapat menghubungkan banyak perangkat (multi-slave) hanya dengan 2 kabel (SDA+SCL), setiap perangkat punya alamat unik, hemat pin |
| ❌ Kerugian | Kecepatan lebih lambat dari SPI, jarak terbatas, implementasi lebih kompleks dari UART, rentan konflik jika dua perangkat punya alamat sama |

---

**Soal 2: Bagaimana peran alamat I2C pada LCD (misalnya 0x27 vs 0x20)? Berikan penjelasan!**

**Jawaban:**
Alamat I2C adalah identitas unik setiap perangkat dalam satu bus I2C, seperti nomor rumah dalam satu jalan. Ketika Arduino ingin berkomunikasi dengan LCD, ia mengirimkan alamat target terlebih dahulu di jalur SDA. Hanya perangkat yang alamatnya cocok yang akan merespons, sedangkan perangkat lain mengabaikannya.

Perbedaan `0x27` vs `0x20` ditentukan oleh chip backpack I2C yang digunakan:
- Chip **NXP PCF8574T** biasanya menggunakan alamat default `0x27`
- Chip **NXP PCF8574AT** atau **TI** biasanya menggunakan alamat `0x20` atau `0x3F`

Alamat ini bisa diubah dengan menyolder pad A0–A2 di belakang modul LCD. Jika salah menuliskan alamat di kode program (`LiquidCrystal_I2C lcd(0x27, 16, 2)`), LCD tidak akan merespons dan layar tetap gelap meskipun koneksi hardware sudah benar. Untuk mengetahui alamat yang tepat, bisa menggunakan sketch **I2C Scanner**.

---

**Soal 3: Jika UART dan I2C digabung dalam satu sistem, bagaimana alur kerja sistem tersebut? Bagaimana Arduino mengelola dua protokol sekaligus?**

**Jawaban:**

**Alur kerja sistem gabungan:**
1. Arduino membaca nilai potensiometer via ADC (`analogRead`)
2. Nilai ADC diproses (dikonversi ke volt dan persen)
3. **UART** → hasil dikirim ke Serial Monitor melalui `Serial.print()`
4. **I2C** → hasil dikirim ke LCD melalui `lcd.print()` yang menggunakan library Wire
5. Kedua output dijalankan secara berurutan dalam satu iterasi `loop()`

**Cara Arduino mengelola dua protokol:**
Arduino dapat mengelola UART dan I2C secara bersamaan karena keduanya menggunakan hardware dan pin yang **berbeda dan terpisah**:
- **UART** menggunakan pin RX (0) dan TX (1) serta hardware USART internal
- **I2C** menggunakan pin A4 (SDA) dan A5 (SCL) serta hardware TWI (Two-Wire Interface) internal

Karena keduanya menggunakan perangkat keras yang berbeda, tidak terjadi konflik. Program cukup memanggil fungsi Serial untuk UART dan fungsi lcd/Wire untuk I2C secara bergantian dalam satu loop. Arduino menangani timing dan sinyal masing-masing protokol secara independen melalui register hardware yang terpisah.