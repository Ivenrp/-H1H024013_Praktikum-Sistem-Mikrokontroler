#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// Inisialisasi LCD sesuai Modul 3 [cite: 196]
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Gunakan lirik yang kamu inginkan
String lirikNina = "Iven Pacar Dina Selingkuhan Masseh "; 

void setup() {
  lcd.init();      // Sesuai prosedur modul [cite: 205]
  lcd.backlight(); // Menyalakan lampu latar [cite: 207]
  
  // Header tetap di baris 0
  lcd.setCursor(3, 0);
  lcd.print("Iven & Dina");
}

void loop() {
  int len = lirikNina.length();
  
  // Looping untuk menggeser teks ke kiri (Running Text)
  for (int posisi = 0; posisi < len; posisi++) {
    
    // Set kursor ke Baris 1 (indeks 1)
    lcd.setCursor(0, 1);
    
    // Menampilkan 16 karakter dari posisi saat ini
    String tampilan = "";
    for (int k = 0; k < 16; k++) {
      // Menggunakan modulo % agar teks berputar (looping) otomatis
      tampilan += lirikNina[(posisi + k) % len];
    }
    
    lcd.print(tampilan);
    
    // Kecepatan gerak teks (semakin kecil, semakin cepat)
    delay(300); 
  }
}
