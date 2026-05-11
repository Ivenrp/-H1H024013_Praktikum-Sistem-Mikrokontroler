#include <Arduino_FreeRTOS.h>

// Deklarasi task
void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void TaskPrint(void *pvParameters);

void setup() {
  // Inisialisasi serial
  Serial.begin(9600);

  // Membuat task
  xTaskCreate(
    TaskBlink1,     // Fungsi task
    "Task1",        // Nama task
    128,            // Stack size
    NULL,           
    1,              // Prioritas
    NULL
  );

  xTaskCreate(
    TaskBlink2,
    "Task2",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskPrint,
    "Task3",
    128,
    NULL,
    1,
    NULL
  );

  // Menjalankan scheduler
  vTaskStartScheduler();
}

void loop() {
  // Kosong karena sudah pakai FreeRTOS
}

// TASK 1 
void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);

  while (1) {
    Serial.println("Task1");
    digitalWrite(8, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);

    digitalWrite(8, LOW);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// TASK 2 
void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);

  while (1) {
    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);

    digitalWrite(7, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// TASK 3 
void TaskPrint(void *pvParameters) {
  int counter = 0;

  while (1) {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}