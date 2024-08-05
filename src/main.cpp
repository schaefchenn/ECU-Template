#include <Arduino.h>
#include "CANBUS.h"

// Core definitions (assuming you have dual-core ESP32)
static const BaseType_t pro_cpu = 0; // protocol core
static const BaseType_t app_cpu = 1; // application core

// Initialize CPU cores
TaskHandle_t Task1;
TaskHandle_t Task2;


//==================================================================================//

void CANBUS_send (void * pvParameters) {
  while (1){
    canSender();
    // yield
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void CANBUS_recieve (void * pvParameters) {
  while (1){
    //canReceiver();
    // yield
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}


//==================================================================================//

void ECU (void * pvParameters){
  while(1){
    // yield
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}


//==================================================================================//

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  while (!Serial);

  // Setup CAN communication and ECU Components
  setupCANBUS();


  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  // Start CANcommunication (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(CANBUS_send,                                  // Function to be called
                          "Controller Area Network Message Sending",    // Name of task
                          4096,                                         // Increased stack size
                          NULL,                                         // Parameter to pass to function
                          2,                                            // Increased priority
                          NULL,                                         // Task handle
                          pro_cpu);                                     // Assign to protocol core

  xTaskCreatePinnedToCore(CANBUS_recieve,                                  // Function to be called
                          "Controller Area Network Message Recieving",  // Name of task
                          4096,                                         // Increased stack size
                          NULL,                                         // Parameter to pass to function
                          2,                                            // Increased priority
                          NULL,                                         // Task handle
                          pro_cpu);                                     // Assign to protocol core

  // Start CANcommunication (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(ECU,                            // Function to be called
                          "Electromic Controll Uni",      // Name of task
                          8192,                           // Increased stack size
                          NULL,                           // Parameter to pass to function
                          2,                              // Increased priority
                          NULL,                           // Task handle
                          app_cpu);                       // Assign to protocol core  
}

void loop() {
  // put your main code here, to run repeatedly:
}