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

// ADC buffer (n samples)
#define BUFFER_SIZE 512

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

volatile uint16_t bufferIndex = 0; 
volatile bool activeBuffer = 0; 
volatile bool isBufferReady = false;

void setupADC() {
  // Timer Setup (1kHz -> 1000 us)
  timer = timerBegin(0, 80, true); // prescaler 80 -> 1000 us
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

  // Set attenuation 11dB -> 0-2.8V
  analogSetAttenuation(ADC_11db);

  // Initialize buffers with zeros
  for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
    bufferA0_0[i] = 0;
    bufferA0_1[i] = 0;
    bufferA1_0[i] = 0;
    bufferA1_1[i] = 0;
  }
}

void adcRead() {
  // Read ADC and save in active buffer
  if (activeBuffer == 1) {
    bufferA0_1[bufferIndex] = analogRead(A0);
    bufferA1_1[bufferIndex] = analogRead(A1);

    // Serial print readings
    Serial.print(bufferA0_1[bufferIndex]);
    Serial.print(", ");
    Serial.print(bufferA1_1[bufferIndex]);
    Serial.print(", ");
  } else {
    bufferA0_0[bufferIndex] = analogRead(A0);
    bufferA1_0[bufferIndex] = analogRead(A1);

    // Serial print readings
    Serial.print(bufferA0_0[bufferIndex]);
    Serial.print(", ");
    Serial.print(bufferA1_0[bufferIndex]);
    Serial.print(", ");
  }

  // Update index and flag when buffer is full
  bufferIndex ++;
  if (bufferIndex >= BUFFER_SIZE) {
    bufferIndex = 0;
    activeBuffer = !(activeBuffer);
    isBufferReady = true; // Tell main loop to write to SD
  }
}

/*

// Moving average sample window
#define N 100

void updateIndex();
void readA0();
void readA1();

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

  // Initialize readings arrays with zeros
  for (uint16_t i = 0; i < N; i++) {
    readingsA0[i] = 0;
    readingsA1[i] = 0;
  }

  void taskADC(){
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
*/

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]