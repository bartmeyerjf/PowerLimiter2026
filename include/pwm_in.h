// ------------------------------------------------------
// PWM Input Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef pwm_in_h
#define pwm_in_h

#include <Arduino.h>
#include "pinconfig.h"

void setupPWMIn();
void IRAM_ATTR isr_pwm_measure();
void taskPWMIn(void *pvParameters);

// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]

// Variables (ISR)
volatile uint32_t riseTime = 0;
volatile uint32_t fallTime = 0;
volatile uint32_t highTime = 0;
volatile uint32_t period = 0;
volatile uint32_t pwmInDuty = 0;

// FreeRTOS Kernel Objects
TaskHandle_t pwmTaskHandle = NULL; 
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR isr_pwm_measure() {
  uint32_t currentTime = micros();
  bool currentState = digitalRead(PIN_PWM_IN);
  
  if (currentState == HIGH) {
    if ((currentTime - riseTime) > 50) { 
      portENTER_CRITICAL_ISR(&mux);
      period = currentTime - riseTime;
      riseTime = currentTime;
      portEXIT_CRITICAL_ISR(&mux);
    }
  } 
  else { 
    if ((currentTime - fallTime) > 50) {
      portENTER_CRITICAL_ISR(&mux);
      fallTime = currentTime;
      highTime = fallTime - riseTime;
      portEXIT_CRITICAL_ISR(&mux);
      
      // FreeRTOS: Activate the math task instantly.
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      vTaskNotifyGiveFromISR(pwmTaskHandle, &xHigherPriorityTaskWoken);
      
      if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR(); // Forces context switching in the processor
      }
    }
  }
}

void taskPWMIn(void *pvParameters) {
  for (;;) {
    // Sleeps indefinitely (blocks) until the ISR sends the signal
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    uint32_t safeHighTime = 0;
    uint32_t safePeriod = 0;
    
    // Critical Section Safe for Multitasking
    portENTER_CRITICAL(&mux);
    safeHighTime = highTime;
    safePeriod = period;
    portEXIT_CRITICAL(&mux);

    if (safePeriod > 0 && safeHighTime <= safePeriod) {
      float frequency = 1000000.0 / (float)safePeriod;
      pwmInDuty = ((16383 * safeHighTime) / safePeriod);

      Serial.print("Period: ");
      Serial.print(safePeriod);
      Serial.print(" us \t | \t High Time: ");
      Serial.print(safeHighTime);
      Serial.print(" us \t | \t Freq: ");
      Serial.print(frequency, 2);
      Serial.print(" Hz \t | \t Duty: ");
      Serial.print((float)pwmInDuty*100/16383, 2);
      Serial.println(" %");
    }
  }
}

void setupPWMIn() { 
  
  pinMode(PIN_PWM_IN, INPUT_PULLDOWN);

  // FreeRTOS: Creates and registers the task in the operating system
  xTaskCreate(
    taskPWMIn,           // Name of the function that contains the task
    "PWM_Task",          // Friendly name for internal kernel debugging
    2048,                // Stack size allocated in Bytes
    NULL,                // Parameters to pass (not applicable here)
    3,                  // Task priority (High priority)
    &pwmTaskHandle       // Passes the memory address of the handle
  );

  attachInterrupt(digitalPinToInterrupt(PIN_PWM_IN), isr_pwm_measure, CHANGE);

}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]