/*
 * FreeRTOS Task Demo
 *
 * Toggles LED and prints "Hello, World" to console in separate tasks.
 *
 * Date: December 3, 2020
 * Author: Shawn Hymel
 * License: 0BSD
 */
 
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Some string to print
const char msg[] = "";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// Task: print to Serial Terminal with lower priority
void startTask1(void *parameter) {
  // Count number of characters in string
  int msg_len = strlen(msg);
  
  // Print string to Terminal
  while (1) {
    Serial.println();
    for (int i = 0; i < msg_len; i++)
      Serial.print(msg[i]);
    
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task: print to Serial Terminal with higher priority
void startTask2(void *parameter) {
  while (1) {
    Serial.print('*');
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  xTaskCreatePinnedToCore(startTask1, "Task 1", 1000, NULL, 1, &task_1, 1);  
  xTaskCreatePinnedToCore(startTask2, "Task 2", 1000, NULL, 1, &task_2, 1);
}

void loop() {}
