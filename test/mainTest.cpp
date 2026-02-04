// ------------------------------------------------------
// File for testing and validation
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef mainTest_cpp
#define mainTest_cpp
// [====================================================]

#include <Arduino.h>
#include "pinconfig.h"
#include "adc.h"

void setupMainTest();
void taskMainTest();


// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

void setupMainTest() {
  Serial.begin(115200);  // begin serial monitor

}

void taskMainTest(){


}

void printAnalogReading() {

  // Print both last value and moving average
  
  Serial.print(" | Voltage average: ");
 // Serial.print(voltageAverage);
  Serial.print("\t");
  Serial.print(" | Voltage value: ");
  //Serial.print(voltageReading);
  Serial.print("\t");
  Serial.println();

}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]