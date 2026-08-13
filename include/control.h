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
#include "plant_model.h"
#include "pwm_out.h"

// #define tau 200000 // x10e-6 s = 200 ms (aproximately)

#define dt 0.01 // time in seconds (10 ms)
#define outMin 1336
#define outMax 2128
const float Kp = 735e-06f;
const float Ki = 47e-3f;

volatile float dutyPI = 0;
volatile float error = 0;
volatile float errorIntegral = 0;
// volatile float errorPrevious = 0;

void taskControl();
void updateDutyFF();
void updateError();
void updateDutyPI();

volatile float dutyFF = 0;
const float ALPHA =  3.8786e-06f;
const float BETA  = -3.4152e-03f;
const float GAMMA =  1.3369e+00f;
const float DELTA =  1.6145e+03f;

// Determine Feed Forward duty cycle
void updateDutyFF(){
    // Horner's Method for fast polynomial calculation:
    // u = ((ALPHA * P + BETA) * P + GAMMA) * P + DELTA
    dutyFF = ((ALPHA * rcPowerSetpoint + BETA) * rcPowerSetpoint + GAMMA) * rcPowerSetpoint + DELTA;
} 

void updateError(){
    error = power - rcPowerSetpoint;
    // Anti-Windup (Clamping) and Output Saturation
    if (dutyPI > outMax) {
        dutyPI = outMax;
        
        // Only allow the integral to update if the error is negative
        // (which will help pull the output back down below the max limit)
        if (error < 0.0f) {
            errorIntegral = errorIntegral + error*dt;;
        }
    } 
    else if (dutyPI < outMin) {
        dutyPI = outMin;
        // Only allow the integral to update if the error is positive
        // (which will help pull the output back up above the min limit)
        if (error > 0.0f) {
            errorIntegral = errorIntegral + error*dt;;
        }
    } 
    else {
        // Output is within bounds; proceed with normal integration
        errorIntegral = errorIntegral + error*dt;;
    }
}

// Determine PI duty cycle
void updateDutyPI(){
    dutyPI = Kp*error + Ki*errorIntegral;
} 

void taskControl(){
    updateDutyFF();
    updateError();
    updateDutyPI();
}

#endif