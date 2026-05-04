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
