// ------------------------------------------------------
// PWM output Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef control_h
#define control_h

#include "adc.h"

#define tau 400000 // x10e-6 s = 400 ms (aproximately)
#define maxPower 600000000 // x10e-6 W = 600 W
volatile uint32_t setPoint;
volatile uint32_t power;

volatile uint32_t timePrevious;
volatile int32_t errorPrevious;
volatile int32_t error = 0;
volatile int32_t errorIntegral = 0;
volatile int32_t errorDerivative = 0;
volatile uint32_t pwmOut;

uint32_t Kp;
uint32_t Ki;
uint32_t Kd;
uint32_t outMin;
uint32_t outMax;

void setupControl(){};
void taskControl(){};


void setupControl(){

}

void taskControl(){
    pwmOut = Kp*error + Ki*errorIntegral + Kd*errorDerivative;
    setPWMOutput(pwmOut);
}

void updateError(){
    errorPrevious = error;
    error = power - setPoint;
    errorIntegral = errorIntegral + error;
    errorDerivative = error - errorPrevious; // assuming a unit time interval
}

#endif