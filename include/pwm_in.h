// ------------------------------------------------------
// PWM Input Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef pwm_in_h
#define pwm_in_h

#include <Arduino.h>
#include "pinconfig.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"
#include "soc/gpio_struct.h"
#include "soc/interrupts.h"   // Correct header for Core 3.x / ESP-IDF 5.x

volatile uint32_t pwmInHighTime_us = 0;
volatile uint32_t pwmInPeriod_us = 0;
volatile uint32_t pwmInLastRiseEdgeInstant = 0;

volatile uint32_t pwmInDuty = 0;
volatile uint32_t pwmInFreq = 0;

// HIGH PRIORITY ISR
void IRAM_ATTR PWMInterrupt(void* arg) {
    // Accessing the .val member is required in Core 3.x
    uint32_t gpio_intr_status = GPIO.status.val;
    
    // Clear the interrupt status
    GPIO.status_w1tc.val = gpio_intr_status;

    if (gpio_intr_status & (1ULL << PIN_PWM_IN)) {
        uint32_t pwmInTime = micros();
        
        if (digitalRead(PIN_PWM_IN) == HIGH) {
            delayMicroseconds(50);
            // RISING EDGE
            pwmInPeriod_us = pwmInTime - pwmInLastRiseEdgeInstant;
            pwmInLastRiseEdgeInstant = pwmInTime;
        } else {
            // FALLING EDGE
            pwmInHighTime_us = pwmInTime - pwmInLastRiseEdgeInstant;
        }
    }
}

void setupPWMIn() {
    // Initialize GPIO
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << PIN_PWM_IN);
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // Allocate Interrupt with Level 3 Priority
    // ETS_GPIO_INTR_SOURCE is found in soc/interrupts.h
    esp_intr_alloc(ETS_GPIO_INTR_SOURCE, 
                   ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM, 
                   PWMInterrupt, 
                   NULL, 
                   NULL);
}

void taskPWMIn() {
    uint32_t localHigh, localPeriod;

    noInterrupts();
    localHigh = pwmInHighTime_us;
    localPeriod = pwmInPeriod_us;
    interrupts();

    if (localPeriod > 0) {
        pwmInDuty = (16383 * localHigh) / localPeriod;
        pwmInFreq = 1000000 / localPeriod;
    }
}

#endif