Modul 4 : Analog to Digital Converter (ADC) dan Pulse Width Modulation (PWM)

---

## SOAL DAN JAWABAN MODUL 4: ADC DAN PWM

---

### 📚 Pertanyaan Praktikum

**Soal 1: Bagaimana proses konversi sinyal analog menjadi digital pada Arduino? Jelaskan!**

**Jawaban:**
Proses konversi sinyal analog menjadi digital pada Arduino menggunakan ADC (Analog to Digital Converter) berjalan sebagai berikut:

1. Arduino Uno memiliki ADC 10-bit yang terdapat pada pin A0–A5
2. Sinyal analog (tegangan) yang masuk ke pin analog dibandingkan dengan tegangan referensi (default 5V)
3. ADC mengukur tegangan input dan mengubahnya menjadi nilai digital menggunakan rumus:

   ```
   Nilai ADC = (Tegangan Input / Tegangan Referensi) × 1023
   ```

4. Hasilnya adalah nilai integer antara **0** (0V) hingga **1023** (5V)
5. Nilai tersebut kemudian dapat dibaca program menggunakan fungsi `analogRead(pin)`
6. Contoh: jika tegangan input 2.5V, maka nilai ADC = (2.5 / 5.0) × 1023 ≈ **511**

---

**Soal 2: Faktor apa saja yang dapat mempengaruhi keakuratan pembacaan ADC?**

**Jawaban:**

Beberapa faktor yang mempengaruhi keakuratan pembacaan ADC antara lain:

| Faktor | Penjelasan |
|--------|-----------|
| **Noise / Gangguan Listrik** | Sinyal interferensi dari lingkungan atau komponen lain dapat menyebabkan fluktuasi nilai ADC yang tidak stabil |
| **Tegangan Referensi** | Jika tegangan referensi (AREF atau AVCC) tidak stabil, hasil konversi menjadi tidak akurat |
| **Resolusi ADC** | Arduino Uno hanya memiliki resolusi 10-bit (1024 langkah), sehingga perubahan tegangan < 4.9 mV tidak terdeteksi |
| **Impedansi Sumber** | Sumber sinyal dengan impedansi tinggi dapat memperlambat proses pengisian kapasitor internal ADC dan menyebabkan pembacaan yang kurang tepat |
| **Kecepatan Sampling** | Jika `analogRead()` dipanggil terlalu cepat, kapasitor internal belum terisi penuh, menyebabkan nilai yang tidak akurat |
| **Suhu** | Perubahan suhu dapat mempengaruhi tegangan referensi internal dan karakteristik komponen |
| **Koneksi Hardware** | Kabel yang longgar atau resistansi jalur yang tinggi dapat memengaruhi tegangan yang sampai ke pin ADC |

---

**Soal 3: Apa kendala yang mungkin terjadi saat mengintegrasikan ADC dan PWM dalam satu sistem?**

**Jawaban:**

Beberapa kendala yang mungkin muncul saat mengintegrasikan ADC dan PWM dalam satu sistem:

| Kendala | Penjelasan |
|---------|-----------|
| **Interferensi sinyal PWM ke ADC** | Sinyal PWM yang bekerja dengan frekuensi tinggi dapat menciptakan noise elektromagnetik yang mengganggu pembacaan ADC dan menyebabkan nilai yang tidak stabil |
| **Konflik penggunaan timer** | PWM pada Arduino menggunakan timer internal. Jika timer yang sama digunakan untuk keperluan lain (seperti `millis()` atau `delay()`), dapat terjadi konflik yang mempengaruhi frekuensi PWM |
| **Keterlambatan respons (latency)** | Jika tidak menggunakan teknik non-blocking (`millis()`), sistem bisa terhenti saat menunggu pembacaan ADC selesai, sehingga output PWM menjadi tidak responsif |
| **Resolusi yang berbeda** | ADC memiliki resolusi 10-bit (0–1023), sedangkan PWM hanya menerima nilai 8-bit (0–255). Perlu konversi menggunakan `map()` agar nilai ADC bisa digunakan langsung oleh `analogWrite()` |
| **Ground loop** | Jika GND tidak terhubung dengan baik antara komponen ADC dan PWM, dapat terjadi perbedaan potensial yang menyebabkan error pembacaan |

---

### 💻 Percobaan 4A: Analog to Digital Converter (ADC)

**Source Code – Kontrol Servo dengan Potensiometer:**

```cpp
#include <Arduino.h>
#include <Servo.h>

// Deklarasi objek servo
Servo myServo;

// Definisi pin
const int PIN_POT   = A0; // Pin potensiometer (input analog)
const int PIN_SERVO = 9;  // Pin sinyal servo (PWM)

void setup() {
  Serial.begin(9600);        // Inisialisasi Serial Monitor dengan baud rate 9600
  myServo.attach(PIN_SERVO); // Hubungkan objek servo ke pin 9
}

void loop() {
  // Baca nilai ADC dari potensiometer (rentang: 0 – 1023)
  int nilaiADC = analogRead(PIN_POT);

  // Konversi nilai ADC ke sudut servo (rentang: 0° – 180°)
  int sudut = map(nilaiADC, 0, 1023, 0, 180);

  // Gerakkan servo ke sudut yang telah dikalkulasi
  myServo.write(sudut);

  // Tampilkan nilai ADC dan sudut ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print("  Sudut: ");
  Serial.print(sudut);
  Serial.println("°");

  delay(15); // Tunggu 15ms agar servo selesai bergerak ke posisi target
}
```

**Penjelasan Kode Percobaan 4A:**

```markdown
### Library
- `Servo.h` : library bawaan Arduino untuk mengendalikan motor servo

### Variabel & Objek
- `myServo`   : objek servo yang merepresentasikan motor servo
- `PIN_POT`   : pin A0 sebagai input pembacaan potensiometer
- `PIN_SERVO` : pin 9 sebagai output sinyal PWM ke servo

### setup()
- `Serial.begin(9600)`      : memulai komunikasi serial untuk monitoring
- `myServo.attach(PIN_SERVO)` : menginisialisasi servo pada pin 9

### loop()
- `analogRead(PIN_POT)`           : membaca nilai tegangan potensiometer (0–1023)
- `map(nilaiADC, 0, 1023, 0, 180)` : mengubah nilai ADC menjadi sudut servo (0°–180°)
- `myServo.write(sudut)`          : menggerakkan servo ke sudut yang diinginkan
- `Serial.print()`                : menampilkan nilai ADC dan sudut ke Serial Monitor
- `delay(15)`                     : jeda 15ms agar servo punya waktu cukup untuk bergerak
```

---

### 💻 Percobaan 4B: Pulse Width Modulation (PWM)

**Source Code – Kontrol Kecerahan LED dengan Potensiometer:**

```cpp
#include <Arduino.h>

// Definisi pin
const int PIN_POT = A0; // Pin potensiometer (input analog)
const int PIN_LED = 9;  // Pin LED (output PWM)

void setup() {
  Serial.begin(9600);        // Inisialisasi Serial Monitor
  pinMode(PIN_LED, OUTPUT);  // Set pin LED sebagai output
}

void loop() {
  // Baca nilai ADC dari potensiometer (rentang: 0 – 1023)
  int nilaiADC = analogRead(PIN_POT);

  // Konversi nilai ADC ke nilai PWM (rentang: 0 – 255)
  int nilaiPWM = map(nilaiADC, 0, 1023, 0, 255);

  // Hitung tegangan (estimasi dari nilai ADC)
  float volt = nilaiADC * (5.0 / 1023.0);

  // Hitung persentase kecerahan
  int persen = map(nilaiADC, 0, 1023, 0, 100);

  // Tulis nilai PWM ke pin LED untuk mengatur kecerahan
  analogWrite(PIN_LED, nilaiPWM);

  // Tampilkan informasi ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print("  PWM: ");
  Serial.print(nilaiPWM);
  Serial.print("  Volt: ");
  Serial.print(volt, 2);
  Serial.print(" V  Kecerahan: ");
  Serial.print(persen);
  Serial.println("%");

  delay(100); // Tunggu 100ms sebelum pembacaan berikutnya
}
```

**Penjelasan Kode Percobaan 4B:**

```markdown
### Variabel
- `PIN_POT` : pin A0 sebagai input analog potensiometer
- `PIN_LED` : pin 9 sebagai output PWM untuk LED

### setup()
- `Serial.begin(9600)`    : memulai komunikasi serial untuk monitoring
- `pinMode(PIN_LED, OUTPUT)` : mengatur pin LED sebagai output digital/PWM

### loop()
- `analogRead(PIN_POT)`          : membaca nilai ADC potensiometer (0–1023)
- `map(nilaiADC, 0, 1023, 0, 255)` : mengkonversi nilai ADC ke range PWM (0–255)
- `nilaiADC * (5.0 / 1023.0)`   : menghitung estimasi tegangan dari nilai ADC
- `map(nilaiADC, 0, 1023, 0, 100)` : menghitung persentase kecerahan LED
- `analogWrite(PIN_LED, nilaiPWM)` : menulis nilai PWM ke pin LED (mengatur kecerahan)
- `Serial.print()`               : menampilkan semua nilai ke Serial Monitor
- `delay(100)`                   : jeda 100ms agar tampilan Serial Monitor tidak terlalu cepat
```

---

**Tabel Hasil Pengamatan Percobaan 4B:**

| ADC | PWM | Volt (V) | Kecerahan (%) |
|-----|-----|----------|---------------|
| 0 | 0 | 0.00 V | 0% |
| 256 | 64 | 1.25 V | 25% |
| 511 | 127 | 2.50 V | 50% |
| 767 | 191 | 3.75 V | 75% |
| 1023 | 255 | 5.00 V | 100% |

> Rumus:  
> PWM = ADC × (255 / 1023)  
> Volt = ADC × (5.0 / 1023)  
> Kecerahan = ADC × (100 / 1023)

---

### 📝 README Tugas Tambahan: Gabungan ADC + PWM

**Source Code – Servo + LED dikontrol satu Potensiometer:**

```cpp
#include <Arduino.h>
#include <Servo.h>

// Objek servo
Servo myServo;

// Definisi pin
const int PIN_POT   = A0; // Pin potensiometer
const int PIN_LED   = 6;  // Pin LED (PWM)
const int PIN_SERVO = 9;  // Pin Servo (PWM)

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  myServo.attach(PIN_SERVO);
}

void loop() {
  // Baca nilai ADC dari potensiometer
  int nilaiADC = analogRead(PIN_POT);

  // Konversi ke sudut servo (0° – 180°)
  int sudut = map(nilaiADC, 0, 1023, 0, 180);

  // Konversi ke nilai PWM untuk LED (0 – 255)
  int nilaiPWM = map(nilaiADC, 0, 1023, 0, 255);

  // Gerakkan servo dan atur kecerahan LED
  myServo.write(sudut);
  analogWrite(PIN_LED, nilaiPWM);

  // Tampilkan ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print("  Sudut: ");
  Serial.print(sudut);
  Serial.print("°  PWM LED: ");
  Serial.println(nilaiPWM);

  delay(15);
}
```

**README.md Tugas Tambahan:**

```markdown
# Modul 4 - Gabungan ADC, PWM, dan Servo

## Penjelasan Kode

### Library yang Digunakan
- `Servo.h` : library untuk mengendalikan motor servo via sinyal PWM

### Variabel & Objek
- `myServo`   : objek yang merepresentasikan motor servo
- `PIN_POT`   : pin A0 sebagai input analog potensiometer
- `PIN_LED`   : pin 6 sebagai output PWM untuk LED
- `PIN_SERVO` : pin 9 sebagai output sinyal PWM ke servo

### setup()
- `Serial.begin(9600)`        : memulai komunikasi serial untuk monitoring
- `pinMode(PIN_LED, OUTPUT)`  : mengatur pin LED sebagai output
- `myServo.attach(PIN_SERVO)` : menginisialisasi servo pada pin 9

### loop()
- `analogRead(PIN_POT)`                 : membaca nilai ADC potensiometer (0–1023)
- `map(nilaiADC, 0, 1023, 0, 180)`     : mengkonversi ADC ke sudut servo
- `map(nilaiADC, 0, 1023, 0, 255)`     : mengkonversi ADC ke nilai PWM LED
- `myServo.write(sudut)`               : menggerakkan servo ke sudut target
- `analogWrite(PIN_LED, nilaiPWM)`     : mengatur kecerahan LED sesuai nilai PWM
- `Serial.print()`                     : menampilkan data ke Serial Monitor
- `delay(15)`                          : jeda 15ms agar servo punya waktu bergerak
```
