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
#include "control.h"
//#include "pwm_in.h"
#include "pwm_out.h"
#include "plant_model.h"

//#include "adc_continuous.h"

#define INTERRUPT_TIME_US 10000 // time in microseconds

void IRAM_ATTR onTimer();

// Timer Interrupt
hw_timer_t *timer = NULL;

// Setup code
void setup() {
  // Turn off wifi and bluetooth
  WiFi.mode(WIFI_OFF);
  btStop();

/* timer = timerBegin(0, 80, true); // prescaler 80 -> 1000 us
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, INTERRUPT_TIME_US, true);
  timerAlarmEnable(timer); */

  // Timer Setup 
  timer = timerBegin(1000000); // Configure the timer to tick at 1 MHz (1 microsecond per tick)
  timerAttachInterrupt(timer, &onTimer); // Attach the interrupt handler
  timerAlarm(timer, INTERRUPT_TIME_US, true, 0); // Set the alarm time, enable auto-reload (true), and reload infinitely (0)

  // Setup libraries
  setupADC();
  //setupPWMIn();
  setupDataLog();
  setupPWMOut();
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Loop code
void loop() {
  //taskPWMIn();
  taskDataLog();
  taskModel();
}

// interrupt code
void IRAM_ATTR onTimer(){
  adcContinuousRead();
  
} 