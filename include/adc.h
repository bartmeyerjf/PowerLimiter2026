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

// ADC buffer (1000 samples)
#define BUFFER_SIZE 1000

// Moving average sample window
#define N 100

void setupADC();
void taskADC();
void adcRead();
void updateIndex();
void readA0();
void readA1();

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Buffers
uint16_t bufferA0_0[BUFFER_SIZE];
uint16_t bufferA0_1[BUFFER_SIZE];
uint16_t bufferA1_0[BUFFER_SIZE];
uint16_t bufferA1_1[BUFFER_SIZE];

volatile uint16_t bufferIndex = 0; 
volatile bool activeBuffer = 0; 
volatile bool isBufferReady = false;

// Mooving average parameters
uint16_t indexMovAvg = 0;

uint16_t readingsA0[N];
uint32_t sumA0 = 0;
uint16_t movAverageA0 = 0;
uint16_t prevMovAverageA0 = 0;

uint16_t readingsA1[N];
uint32_t sumA1 = 0;
uint16_t movAverageA1 = 0;
uint16_t prevMovAverageA1 = 0;

void setupADC() {
  // Set attenuation 11dB -> 0-2.5V
  analogSetAttenuation(ADC_11db);

  // Initialize readings arrays with zeros
  for (uint16_t i = 0; i < N; i++) {
    readingsA0[i] = 0;
    readingsA1[i] = 0;
  }
}

void taskADC(){
  updateIndex();
  readA0();
  readA1();
}

void adcRead() {
  if (activeBuffer == 1) {
    bufferA0_1[bufferIndex] = analogRead(A0);
    bufferA1_1[bufferIndex] = analogRead(A1);
  } else {
    bufferA0_0[bufferIndex] = analogRead(A0);
    bufferA1_0[bufferIndex] = analogRead(A1);
  }

  bufferIndex ++;
  if (bufferIndex >= BUFFER_SIZE) {
    bufferIndex = 0;
    activeBuffer = !(activeBuffer);
    isBufferReady = true; // Tell main loop to write to SD
  }

}

void readA0() {
  // remove oldest reading from sum
  sumA0 = sumA0 - readingsA0[indexMovAvg];
  // update readings array
  readingsA0[indexMovAvg] = analogRead(A0);
  // add new reading to sum
  sumA0 = sumA0 + readingsA0[indexMovAvg];
  // compute moving average
  prevMovAverageA0 = movAverageA0;
  movAverageA0 = sumA0/N;
}

void readA1() {
  // remove oldest reading from sum
  sumA1 = sumA1 - readingsA1[indexMovAvg];
  // update readings array
  readingsA1[indexMovAvg] = analogRead(A1);
  // add new reading to sum
  sumA1 = sumA1 + readingsA1[indexMovAvg];
  // compute moving average
  prevMovAverageA1 = movAverageA1;
  movAverageA1 = sumA1/N;
}

void updateIndex(){
  // Update index position
  indexMovAvg = (indexMovAvg + 1) % N;
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]