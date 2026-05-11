#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Struktur data
struct readings {
  int temp;
  int h;
};

// Handle queue
QueueHandle_t my_queue;

// Deklarasi task
void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // Membuat queue (kapasitas 1 data)
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Membuat task
  xTaskCreate(
    read_data,
    "Read Sensors",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    display,
    "Display",
    128,
    NULL,
    1,
    NULL
  );
}

void loop() {
  // Kosong karena pakai FreeRTOS
}

// TASK READ
void read_data(void *pvParameters) {
  struct readings x;

  for (;;) {
    x.temp = 54;
    x.h    = 30;

    xQueueSend(my_queue, &x, portMAX_DELAY);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// TASK DISPLAY
void display(void *pvParameters) {
  struct readings x;

  for (;;) {
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("temp = ");
      Serial.println(x.temp);

      Serial.print("humidity = ");
      Serial.println(x.h);
    }
  }
}