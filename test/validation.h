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
#include <stdlib.h>
#include "pinconfig.h"
#include "adc.h"
#include "lut.h"

void printHeader();
void printA0();
void printA1();

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

void setupValidation() {
    Serial.begin(115200);  // begin serial monitor
    //printHeader();
}

void taskValidation(){

    if(abs(prevMovAverageA0-movAverageA0)>4 || abs(prevMovAverageA1-movAverageA1)>4){
    Serial.print(micros());
    Serial.print("\t | \t ");
    Serial.print(indexPosition);
    Serial.print("\t | \t ");
    printA0();
    printA1();
    Serial.println();
    }

}

void printHeader(){
    Serial.print("Time (us)  \t | \t ");
    Serial.print("Reading A0 \t | \t Mov Avg A0 \t | \t Pin (V) A0 \t | \t ");
    Serial.print("Reading A1 \t | \t Mov Avg A1 \t | \t Pin (V) A1 \t | \t ");
    Serial.println();
}

void printA0(){
    Serial.print(readingsA0[indexPosition]);
    Serial.print(" \t| MA0 ");
    Serial.print(movAverageA0);
    //Serial.print(" \t | VA0 ");
    //Serial.print(lutPinVoltageA0);
    Serial.print(" \t ");
}

void printA1(){
    Serial.print(readingsA1[indexPosition]);
    Serial.print(" \t | MA1 ");
    Serial.print(movAverageA1);
    //Serial.print(" \t | VA1 ");
    //Serial.print(lutPinVoltageA1);
    Serial.print(" \t ");
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]