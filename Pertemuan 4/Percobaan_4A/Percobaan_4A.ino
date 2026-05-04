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
