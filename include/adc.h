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

// Moving average sample window
#define N 256

// Exponential average coeficient (from 1-256)
#define alpha 16

void setupADC();
void taskADC();
void updateIndex();
void readA0();
void readA1();
//void readA2();
//void readA3();
//void readA4();


// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Mooving average parameters
uint16_t indexPosition = 0;
uint16_t readingsA0[N];
uint32_t sumA0 = 0;
uint16_t movAverageA0 = 0;
//uint16_t expAverageA0 = 0;
uint16_t readingsA1[N];
uint32_t sumA1 = 0;
uint16_t movAverageA1 = 0;
//uint16_t expAverageA1 = 0;

void setupADC() {
  // Set attenuation 11dB -> 0-2.5V
  analogSetAttenuation(ADC_11db);

  // Initialize readingsA0 with zeros
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

void readA0() {
  // remove oldest reading from sum
  sumA0 = sumA0 - readingsA0[indexPosition];
  // update readings array
  readingsA0[indexPosition] = analogRead(A0);
  // add new reading to sum
  sumA0 = sumA0 + readingsA0[indexPosition];
  // compute moving average
  movAverageA0 = sumA0/N;
  // compute exponential average
  //expAverageA0 = (readingsA0[indexPosition]*alpha + expAverageA0*(255-alpha))/255;
}

void readA1() {
  // remove oldest reading from sum
  sumA1 = sumA1 - readingsA1[indexPosition];
  // update readings array
  readingsA1[indexPosition] = analogRead(A1);
  // add new reading to sum
  sumA1 = sumA1 + readingsA1[indexPosition];
  // compute moving average
  movAverageA1 = sumA1/N;
  // compute exponential average
  //expAverageA1 = (readingsA1[indexPosition]*alpha + expAverageA1*(256-alpha))/256;
}

void updateIndex(){
  // Update index position
  indexPosition = (indexPosition + 1) % N;
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]