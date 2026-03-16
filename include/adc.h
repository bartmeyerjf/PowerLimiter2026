// ------------------------------------------------------
// Analog to Digital Converter Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef adc_h
#define adc_h
// [====================================================]

#include <Arduino.h>
#include "pinconfig.h"

#include "pwm_in.h"

// ADC buffer (n samples)
#define BUFFER_SIZE 512

volatile uint16_t currentReading = 0;
volatile float currentReadingmV = 0;

void setupADC();
void adcRead();

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Buffers
uint16_t bufferA0_0[BUFFER_SIZE];
uint16_t bufferA0_1[BUFFER_SIZE];
uint16_t bufferA1_0[BUFFER_SIZE];
uint16_t bufferA1_1[BUFFER_SIZE];
uint32_t timeStamp_0[BUFFER_SIZE];
uint32_t timeStamp_1[BUFFER_SIZE];
uint32_t dutyCycle[BUFFER_SIZE];

volatile uint16_t bufferIndex = 0; 
volatile bool activeBuffer = 0; 
volatile bool isBufferReady = false;

void setupADC() {
  // Set attenuation 11dB -> 0-2.8V
  analogSetAttenuation(ADC_11db);

  // Initialize buffers with zeros
  for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
    bufferA0_0[i] = 0;
    bufferA0_1[i] = 0;
    bufferA1_0[i] = 0;
    bufferA1_1[i] = 0;
    timeStamp_0[i] = 0;
    timeStamp_1[i] = 0;
    dutyCycle[i] = 0;
  }
}

void adcRead() {
  // Read ADC and save in active buffer
  if (activeBuffer == 1) {
    bufferA0_1[bufferIndex] = analogRead(A0);
    bufferA1_1[bufferIndex] = analogRead(A1);
    timeStamp_1[bufferIndex] = micros();
    dutyCycle[bufferIndex] = (pwmInDuty);
  } else {
    bufferA0_0[bufferIndex] = analogRead(A0);
    bufferA1_0[bufferIndex] = analogRead(A1);
    timeStamp_0[bufferIndex] = micros();
    dutyCycle[bufferIndex] = (pwmInDuty);
  }

/*   currentReading = analogRead(A1);
  currentReadingmV = analogReadMilliVolts(A1); */

  // Update index and flag when buffer is full
  bufferIndex ++;
  if (bufferIndex >= BUFFER_SIZE) {
    bufferIndex = 0;
    activeBuffer = !(activeBuffer);
    isBufferReady = true; // Tell main loop to write to SD
  }
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]