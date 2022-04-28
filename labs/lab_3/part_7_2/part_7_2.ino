#include <WiFi.h>

static const BaseType_t core_zero = 0;
static const BaseType_t core_one = 1;

int LED_BUILTIN = 2;
int LED_EXTENAL = 27;

static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// credentials for the hotsopt
const char* ssid     = "EN20421380";
const char* password = "password";

WiFiServer server(80);

void connect_wifi(void *parameters) {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
    
  server.begin();

  // start task 2
  xTaskCreatePinnedToCore(web_server, "Web Server", 4096, NULL, 1, &task_2, core_one);

  // delete task 1
  vTaskDelete(task_1);
}

void web_server(void *parameters) {
  while(1) {
    
    WiFiClient client = server.available();   
    
    if (client) {                             
      Serial.println("New Client.");          
      String currentLine = "";                
      
      while (client.connected()) {            
        
        if (client.available()) {           
          char c = client.read();             
          Serial.write(c); 
                            
          if (c == '\n') {                    
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              
              client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
              client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");
  
              client.println();
              break;
            
            } else {   
              currentLine = "";
            }
          
          } else if (c != '\r') {  
            currentLine += c;      
          }
  
          if (currentLine.endsWith("GET /H")) {
            digitalWrite(LED_BUILTIN, HIGH); 
            digitalWrite(LED_EXTENAL, HIGH);               
          }
          
          if (currentLine.endsWith("GET /L")) {
            digitalWrite(LED_BUILTIN, LOW);  
            digitalWrite(LED_EXTENAL, LOW);               
          }
          
        }
      }
      
      client.stop();
      Serial.println("Client Disconnected.");
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_EXTENAL, OUTPUT);

  // Start task A
  xTaskCreatePinnedToCore(connect_wifi, "Connect WiFi", 4096, NULL, 1, &task_1, core_zero);
  
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {}
