int LED_BUILTIN = 2;
TaskHandle_t Task1, Task2;

int task_one_delay = 0;
int task_two_delay = 0;

void task1(void* parameters) {
  while(1) {
    digitalWrite(LED_BUILTIN, HIGH);   
    vTaskDelay( *((int*)parameters) / portTICK_PERIOD_MS);                     
    digitalWrite(LED_BUILTIN, LOW);    
    vTaskDelay( *((int*)parameters) / portTICK_PERIOD_MS);  
  }
}

void task2(void* parameters) {
  while(1) {
    digitalWrite(LED_BUILTIN, HIGH);   
    vTaskDelay( *((int*)parameters) / portTICK_PERIOD_MS);                     
    digitalWrite(LED_BUILTIN, LOW);    
    vTaskDelay( *((int*)parameters) / portTICK_PERIOD_MS);  
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

void loop() {  
  Serial.print("Input task one delay: ");
  while(!Serial.available());
  task_one_delay = Serial.read();  
  delay(5000); 

  Serial.println("Input task two delay: ");
  while(!Serial.available());
  task_two_delay = Serial.read(); 
  delay(5000); 
  
  xTaskCreatePinnedToCore(task1, "Task 1", 1000, &task_one_delay, 1, &Task1, 1);  
  xTaskCreatePinnedToCore(task2, "Task 2", 1000, &task_two_delay, 1, &Task2, 1);

  while(1);
}
