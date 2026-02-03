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

void setupADC();
void taskADC();
void readVoltage();
void readCurrent();


// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

const int N = 500;
int readings[N];
int indexPos = 0;
int sum = 0;

void setupADC() {
  Serial.begin(115200);  // begin serial monitor
  analogSetAttenuation(ADC_11db);


  // Initialize readings with zeros
  for (int i = 0; i < N; i++) {
    readings[i] = 0;
  }
}

void taskADC(){

  readVoltage();
  readCurrent();

}

void readVoltage() {

  int voltageReading = analogReadMilliVolts(analogReadPinVoltage);
  sum = sum - readings[indexPos] + voltageReading;
  readings[indexPos] = voltageReading;

  // Update position
  indexPos = (indexPos + 1) % N;

  // Compute moving average
  float voltageAverage = (float)sum/N;

  // Print both last value and moving average
  
  Serial.print(" | Voltage average: ");
  Serial.print(voltageAverage);
  Serial.print("\t");
  Serial.print(" | Voltage value: ");
  Serial.println(voltageReading);

}

void readCurrent() {

  int currentReading = analogReadMilliVolts(analogReadPinCurrent);
  sum = sum - readings[indexPos] + currentReading;
  readings[indexPos] = currentReading;

  // Update position
  indexPos = (indexPos + 1) % N;

  // Compute moving average
  float currentAverage = (float)sum/N;

  // Print both last value and moving average
  
  Serial.print(" | Current average: ");
  Serial.print(currentAverage);
  Serial.print("\t");
  Serial.print(" | Current value: ");
  Serial.println(currentReading);

}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]