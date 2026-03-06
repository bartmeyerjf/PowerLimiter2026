// ------------------------------------------------------
// PWM Input Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef pwm_in_h
#define pwm_in_h
// [====================================================]

#include <Arduino.h>
#include "pinconfig.h"

#include "esp_intr_alloc.h"
#include "esp_attr.h"

volatile uint32_t pwmInHighTime_us = 0;
volatile uint32_t pwmInPeriod_us = 0;
volatile uint32_t pwmInLastRiseEdgeInstant = 0;
volatile uint32_t pwmInTime = 0;
//volatile bool pwmInUpdate = false;

volatile float pwmInDuty = 0;
volatile uint32_t pwmInFreq = 0;

void setupPWMIn();
void taskPWMIn();

void IRAM_ATTR PWMInterrupt() {
    pwmInTime = micros();
      if (digitalRead(PIN_PWM_IN) == HIGH) {
        // RISING EDGE
        pwmInPeriod_us = pwmInTime - pwmInLastRiseEdgeInstant;
        pwmInLastRiseEdgeInstant = pwmInTime;
    } else {
        // FALLING EDGE
        pwmInHighTime_us = pwmInTime - pwmInLastRiseEdgeInstant;
    }
}

void setupPWMIn(){

    pinMode(PIN_PWM_IN, INPUT_PULLDOWN); 
    // Trigger on CHANGE so we see both Rising and Falling edges
    attachInterrupt(digitalPinToInterrupt(PIN_PWM_IN), PWMInterrupt, CHANGE);

}


// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]

void taskPWMIn(){
  pwmInDuty = (float)pwmInHighTime_us/(float)pwmInPeriod_us;
  pwmInFreq = 1000000/(float)pwmInPeriod_us;

}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]