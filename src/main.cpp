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
#include "datalog.h"
//#include "pwm.h"

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

  // Timer Setup (1kHz -> 1000 us)
  timer = timerBegin(0, 80, true); // prescaler 80 -> 1000 us
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

  setupADC();
  setupDataLog();
  //setupPWM();

}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Loop code
void loop() {
  taskDataLog();
  //vTaskDelete(NULL); 
}

void IRAM_ATTR onTimer(){
  adcRead();
}