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
//#define outMin 1336
//#define outMax 2128
#define outMin 0
#define outMax 1
//const float Kp = 735e-06f;
//const float Ki = 47e-3f;

//const float Kp = 1.6;
//const float Ki = 0.0873;

const float Kp = 0.001;
const float Ki = 0.002;

//const float Kp = 0.0015;
//const float Ki = 0.05;

volatile float power = 0; // power in Watts
volatile float dutyControl = 0;

volatile float dutyPI = 0;
volatile float error = 0;
volatile float errorIntegral = 0;
volatile float auxDutyPI;
// volatile float errorPrevious = 0;

void taskControl();
void updateDutyFF();
void updateError();
void updateDutyPI();

volatile float dutyFF = 0;
const float ALPHA =  0.0702e-06f;
const float BETA  = -0.0860e-03f;
const float GAMMA =  0.0049e+00f;
const float DELTA = -0.0017e+03f;

//const float ALPHA =  0.0984e-06f;
//const float BETA  = -0.0869e-03f;
//const float GAMMA =  0.0034e+00f;
//const float DELTA = -0.0030e+03f;

//const float ALPHA =  3.8786e-06f;
//const float BETA  = -3.4152e-03f;
//const float GAMMA =  1.3369e+00f;
//const float DELTA =  1.6145e+03f;

volatile bool updateControl = 0;

// Determine Feed Forward duty cycle
void updateDutyFF(){
    // Horner's Method for fast polynomial calculation:
    // u = ((ALPHA * P + BETA) * P + GAMMA) * P + DELTA
    dutyFF = ((ALPHA * rcPowerSetpoint + BETA) * rcPowerSetpoint + GAMMA) * rcPowerSetpoint + DELTA;
} 

// Determine PI duty cycle
void updateDutyPI(){
    error = rcPowerSetpoint - power;
    // Anti-Windup (Clamping) and Output Saturation
    float auxDutyPI = Kp*error + Ki*(errorIntegral + error*dt);
    if (auxDutyPI > outMax) {
        dutyPI = outMax;
        // Only allow the integral to update if the error is negative
        // (which will help pull the output back down below the max limit)
        if (error < 0.0f) {
            errorIntegral = errorIntegral + error*dt;
        }
    } 
    else if (auxDutyPI < outMin) {
        dutyPI = outMin;
        // Only allow the integral to update if the error is positive
        // (which will help pull the output back up above the min limit)
        if (error > 0.0f) {
            errorIntegral = errorIntegral + error*dt;
        }
    } 
    else {
        // Output is within bounds; proceed with normal integration
        errorIntegral = errorIntegral + error*dt;
        dutyPI = Kp*error + Ki*errorIntegral;
    }
}

// Determine control duty cycle
void updateDutyControl(){
    // update feedforward and PI control
    updateDutyFF();
    updateDutyPI();
    // Combines feedforward and PI control
    //dutyControl = dutyPercentageToValue(dutyFF);
    //dutyControl = dutyPercentageToValue(dutyFF + dutyPI);
    dutyControl = (dutyPI);

    // Locks duty control output within min and max range
    if (dutyControl > outMax) {
        dutyControl = outMax;
    } else if (dutyControl < outMin) {
        dutyControl = outMin;
    }
} 

void taskControl(){
    if (updateControl) {
        updateDutyControl();
        updateControl = 0; // flag to tell program control effort value has been updated
    }
    
    if((micros() < timeStart + t0) || (micros() > timeEnd + t0)){
    dutyControl = 0;
    updateControl = 0;
    }

}

#endif