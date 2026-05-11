Modul 5 : Multitasking -- Komunikasi Task

---

## SOAL DAN JAWABAN MODUL 5: Multitasking -- Komunikasi Task

---

### 📚 Pertanyaan Praktikum Modul 5: Percobaan 5A (Multitasking)

**Soal 1: Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!**

**Jawaban:**
Ketiga task tidak berjalan secara benar-benar bersamaan (parallel), melainkan bergantian secara sangat cepat (concurrent). FreeRTOS scheduler menggunakan algoritma preemptif berbasis prioritas: saat TaskBlink1 memanggil vTaskDelay(200ms), task tersebut masuk ke state Blocked dan CPU dialihkan ke TaskBlink2 atau Taskprint yang sedang Ready. Proses pergantian ini terjadi sangat cepat sehingga terkesan berjalan bersamaan. Karena ketiganya memiliki prioritas sama (1), scheduler membagi CPU secara round-robin

**Soal 2: Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!**


**Jawaban:**

Menambahkan task keempat dilakukan dengan: (a) mendeklarasikan prototipe fungsi, misalnya void TaskSensor(void *pvParameters); (b) memanggil xTaskCreate(TaskSensor, "task4", 128, NULL, 1, NULL) di dalam setup() sebelum vTaskStartScheduler(); (c) mendefinisikan fungsi TaskSensor dengan loop while(1) dan vTaskDelay() di dalam tubuh fungsi tersebut.


**Soal 3: Program Modifikasi (Kontrol Kecepatan LED dengan Potensiometer)*

**Jawaban:**

Modifikasi dengan potensiometer: ditambahkan TaskPot yang membaca nilai ADC dari potensiometer setiap 100ms menggunakan analogRead(A0), lalu memetakan nilai tersebut ke delay LED menggunakan map(). Nilai ADC besar → delay kecil (kedip cepat), nilai ADC kecil → delay besar (kedip lambat). Hasilnya LED berkedip dengan kecepatan yang dikendalikan oleh putaran potensiometer secara real-time. Penjelasan tiap baris kode terlampir dalam README.md.

```
#include <Arduino_FreeRTOS.h>

// Deklarasi task
void TaskBlinkLED(void *pvParameters);
void TaskReadPotensio(void *pvParameters);

// Variabel global untuk menyimpan delay LED
int ledDelay = 500; 
const int potPin = A0; // Pin analog potensiometer
const int ledPin = 8;  // Pin LED

void setup() {
  Serial.begin(9600);

  // Membuat Task 1: Mengontrol Kedipan LED
  xTaskCreate(TaskBlinkLED, "BlinkLED", 128, NULL, 1, NULL);
  
  // Membuat Task 2: Membaca Potensiometer
  xTaskCreate(TaskReadPotensio, "ReadPot", 128, NULL, 1, NULL);

  // Memulai scheduler RTOS
  vTaskStartScheduler();
}

void loop() {
  // Kosong karena scheduler RTOS sudah mengambil alih loop utama
}

// Implementasi Task 1
void TaskBlinkLED(void *pvParameters) {
  pinMode(ledPin, OUTPUT);
  while(1) {
    digitalWrite(ledPin, HIGH);
    // Delay sesuai dengan variabel global yang diubah potensiometer
    vTaskDelay(ledDelay / portTICK_PERIOD_MS); 
    
    digitalWrite(ledPin, LOW);
    vTaskDelay(ledDelay / portTICK_PERIOD_MS);
  }
}

// Implementasi Task 2
void TaskReadPotensio(void *pvParameters) {
  while(1) {
    int potValue = analogRead(potPin); // Membaca nilai potensiometer (0-1023)
    
    // Mapping nilai potensiometer menjadi delay (misal: 50ms - 1000ms)
    ledDelay = map(potValue, 0, 1023, 50, 1000); 
    
    Serial.print("Nilai Potensiometer: ");
    Serial.print(potValue);
    Serial.print(" | Delay LED: ");
    Serial.println(ledDelay);
    
    // Memberikan jeda pembacaan ADC (Context Switch)
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

```

### 💻 Percobaan 4A: Analog to Digital Converter (ADC)

**1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!**

**Jawaban:**

Kedua task berjalan secara concurrent (bergantian), bukan benar-benar paralel. Task read_data mengirim data ke queue lalu memanggil vTaskDelay(100ms) sehingga masuk state Blocked. Selama itu, task display aktif menunggu data dari queue dengan xQueueReceive (portMAX_DELAY). Begitu data tersedia di queue, task display bangun dari state Blocked, membaca data, menampilkannya ke Serial Monitor, lalu kembali menunggu. Siklus ini bergantian dikelola oleh FreeRTOS scheduler.

**2. Apakah program ini berpotensi mengalami race condition? Jelaskan!**


**Jawaban:**

Program ini tidak berpotensi mengalami race condition karena menggunakan message queue yang dikelola FreeRTOS secara thread-safe. Queue menjamin bahwa hanya satu task yang dapat mengakses data pada satu waktu xQueueSend() dan xQueueReceive() menggunakan mekanisme internal critical section. Race condition hanya bisa terjadi jika dua task mengakses variabel global bersama tanpa proteksi mutex atau semaphore, yang tidak terjadi pada program ini.



**3. Program Modifikasi (Menggunakan Sensor DHT Sesungguhnya)**


**Jawaban:**

```

#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include "DHT.h" // Menambahkan library sensor DHT

#define DHTPIN 2      // Menentukan pin yang terhubung ke sensor DHT
#define DHTTYPE DHT11 // Menentukan tipe DHT (DHT11/DHT22)

DHT dht(DHTPIN, DHTTYPE); // Membuat objek dht

// Modifikasi struct: menggunakan tipe data float untuk nilai desimal sensor
struct readings {
  float temp;
  float h;
};

QueueHandle_t my_queue;

void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(9600);
  dht.begin(); // Menginisialisasi pembacaan sensor DHT

  // Membuat queue dengan kapasitas 1 item dari struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));
  
  if (my_queue != NULL) {
    // Membuat task dengan prioritas yang sama
    xTaskCreate(read_data, "ReadSensors", 128, NULL, 1, NULL);
    xTaskCreate(display, "Display", 128, NULL, 1, NULL);
  }
}

void loop() {
  // Loop dibiarkan kosong karena scheduler RTOS yang berjalan
}

void read_data(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Membaca suhu dan kelembaban aktual dari sensor
    x.temp = dht.readTemperature();
    x.h = dht.readHumidity();

    // Error handling sederhana jika sensor gagal dibaca
    if (isnan(x.temp) || isnan(x.h)) {
      x.temp = 0.0;
      x.h = 0.0;
    }

    // Mengirim struct data ke queue
    xQueueSend(my_queue, &x, portMAX_DELAY);
    
    // Memberikan jeda karena sensor DHT membutuhkan waktu untuk update data
    vTaskDelay(2000 / portTICK_PERIOD_MS); 
  }
}

void display(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Menunggu kiriman data dari queue
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      // Menampilkan output yang sudah dinamis
      Serial.print("Suhu = ");
      Serial.print(x.temp);
      Serial.print(" *C | Kelembaban = ");
      Serial.print(x.h);
      Serial.println(" %");
    }
  }
}
```