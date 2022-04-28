static const BaseType_t core_zero = 0;
static const BaseType_t core_one = 1;

void task_A(void *parameters) {
  while (1) {
    Serial.println("I am Task A I am Running on Core_" + String(xPortGetCoreID()));
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task_B(void *parameters) {
  while (1) {
    Serial.println("I am Task B I am Running on Core_" + String(xPortGetCoreID()));
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // Start task A
  xTaskCreatePinnedToCore(task_A, "Task A", 1024, NULL, 1, NULL, core_zero);

  // Start task B
  xTaskCreatePinnedToCore(task_B, "Task B", 1024, NULL, 1, NULL, core_one);
  
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {}
