// ------------------------------------------------------
// Main Project File
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]

// Include
#include <Arduino.h>
#include "pinconfig.h"
#include "adc.h"
#include "lut.h"
#include "validation.h"

void IRAM_ATTR onTimer();

// Timer Interrupt
hw_timer_t *timer = NULL;

// Test sync
// Setup code
void setup() {
  setupADC();
  setupValidation();

  // Timer Setup (1kHz -> 1000 us)
  timer = timerBegin(0, 80, true); // prescaler 80 -> 1000 us
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Loop code
void loop() {
  taskADC();
  taskLUT();
  taskValidation();
}

void IRAM_ATTR onTimer(){
  adcRead();
}