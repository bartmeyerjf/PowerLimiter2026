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

#include "pwm_in.h"
#include "pwm_out.h"

//#include "lut.h"
//#include "validation.h"

void IRAM_ATTR onTimer();

// Timer Interrupt
hw_timer_t *timer = NULL;

// Setup code
void setup() {
  // Start serial monitor
  Serial.begin(460800);
  // Turn off wifi and bluetooth
  WiFi.mode(WIFI_OFF);
  btStop();

  // Timer Setup (1kHz -> 1000 us)
  timer = timerBegin(0, 80, true); // prescaler 80 -> 1000 us
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

  // Setup libraries
  setupADC();
  setupPWMIn();
  setupPWMOut();
  setupDataLog();
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Loop code
void loop() {
  taskPWMIn();
  taskDataLog();
  // Set pwm out to be the same as pwm in
  setPWMOutput(0.10);

}

// 1 kHz interrupt code
void IRAM_ATTR onTimer(){
  adcRead();
}