// ------------------------------------------------------
// Main Project File
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]

// Include
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>

#include "pinconfig.h"
#include "adc.h"
#include "pwm.h"

//#include "lut.h"
//#include "validation.h"

void IRAM_ATTR onTimer();

// Timer Interrupt
hw_timer_t *timer = NULL;

// Test sync
// Setup code
void setup() {
  Serial.begin(460800);
  WiFi.mode(WIFI_OFF);
  btStop();

  setupADC();
  setupPWM();

}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Loop code
void loop() {
  vTaskDelete(NULL); 
}

void IRAM_ATTR onTimer(){
  Serial.print(micros());
  Serial.print(", ");
  adcRead();
  Serial.println();
}