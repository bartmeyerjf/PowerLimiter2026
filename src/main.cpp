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
#include "datafit.h"
#include "adc.h"
#include "datalog.h"

#include "pwm_in.h"
#include "pwm_out.h"

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
  setupDataLog();
  setupPWMOut();

  // starter value for pwm output
  //setPWMOutput((uint32_t)(8.16*16383/100));
  
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Loop code
void loop() {

  taskPWMIn();
  taskDataLog();

  //setPWMOutput((uint32_t)(dutyOutput*16383/100));
  
}

// 1 kHz interrupt code
void IRAM_ATTR onTimer(){
  adcRead();
  
} 