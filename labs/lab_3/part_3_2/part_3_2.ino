#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

int LED_BUILTIN = 2;
TaskHandle_t Task1, Task2;

void task1(void* parameters) {
  while(1) {
    digitalWrite(LED_BUILTIN, HIGH);   
    vTaskDelay(500 / portTICK_PERIOD_MS);                     
    digitalWrite(LED_BUILTIN, LOW);    
    vTaskDelay(500 / portTICK_PERIOD_MS);  
  }
}

void task2(void* parameters) {
  while(1) {
    digitalWrite(LED_BUILTIN, HIGH);   
    vTaskDelay(300 / portTICK_PERIOD_MS);                     
    digitalWrite(LED_BUILTIN, LOW);    
    vTaskDelay(300 / portTICK_PERIOD_MS);  
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  xTaskCreatePinnedToCore(task1, "Task 1", 1000, NULL, 1, &Task1, 1);  
  xTaskCreatePinnedToCore(task2, "Task 2", 1000, NULL, 1, &Task2, 1);
}

void loop() {}
