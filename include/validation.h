// ------------------------------------------------------
// Console Data validation Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef validation_h
#define validation_h
// [====================================================]

#include <Arduino.h>
#include "pinconfig.h"
#include "adc.h"

void printHeader();
void printA0();
void printA1();

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

void setupValidation() {
    Serial.begin(115200);  // begin serial monitor
    printHeader();
}

void taskValidation(){
    printA0();
    printA1();
}

void printHeader(){
    Serial.print("Reading A0 \t | \t Mov Avg A0 \t | \t Exp Avg A0 \t | \t ");
    Serial.print("Reading A1 \t | \t Mov Avg A1 \t | \t Exp Avg A1 \t | \t ");
    Serial.println();
}

void printA0(){
    Serial.print(readingsA0[indexPosition]);
    Serial.print(" \t | \t ");
    Serial.print(movAverageA0);
    Serial.print(" \t | \t ");
    Serial.print(expAverageA0);
    Serial.print(" \t | \t ");
}

void printA1(){
    Serial.print(readingsA1[indexPosition]);
    Serial.print(" \t | \t ");
    Serial.print(movAverageA1);
    Serial.print(" \t | \t ");
    Serial.print(expAverageA1);
    Serial.print(" \t | \t ");
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]