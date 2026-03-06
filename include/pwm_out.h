// ------------------------------------------------------
// PWM output Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef pwm_out_h
#define pwm_out_h

#include <Arduino.h>
#include "pinconfig.h"

// Configuration Constants
const float PWM_FREQ = 73;
const uint8_t  PWM_RES = 12;   // 12-bit resolution (0-4095)
const uint8_t  PWM_CHAN = 0;   // LEDC Channel 0

//Initializes the Hardware PWM Output
void setupPWMOut() {
    // Board manager version (2.x)
    // Configure timer channel (Channel, Frequency, Resolution)
    ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
    // Attach GPIO pin to configured channel
    ledcAttachPin(PIN_PWM_OUT, PWM_CHAN);
}

void setPWMOutput(float dutyPercentage) {
    // Constrain duty to 0.0 - 1.0 range
    if (dutyPercentage < 0.0f) {dutyPercentage = 0.0f;}
    if (dutyPercentage > 1.0f) {dutyPercentage = 1.0f;}

    // Calculate value based on 12-bit resolution (2^12 = 4096)
    uint16_t pwmOutDutyValue = (uint16_t)(dutyPercentage * 4095.0f);

    // Write to hardware - this is non-blocking
    ledcWrite(PIN_PWM_OUT, pwmOutDutyValue);
}

#endif