// ------------------------------------------------------
// Look Up Table
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef lut_h
#define lut_h
// [====================================================]

#include <Arduino.h>
#include "pinconfig.h"
#include "adc.h"

#define DIM0 32
#define DIM1 16

void lutA0(uint16_t x);
void lutA1(uint16_t x);
void taskLUT();

const uint16_t refMesurementA0[DIM0] = {};  // 12 bit value
const uint16_t refMesurementA1[DIM1] = {};  // 12 bit value
const uint32_t refPinVoltageA0[DIM0] = {};  // microvolts
const uint32_t refPinVoltageA1[DIM1] = {};  // microvolts
const uint32_t refSensorValueA0[DIM0] = {}; // microvolts
const uint32_t refSensorValueA1[DIM1] = {}; // microvolts

uint8_t lutIndexA0 = DIM0-1;
uint8_t lutIndexA1 = DIM1-1;
uint32_t lutPinVoltageA0 = 0;  // microvolts
uint32_t lutPinVoltageA1 = 0;  // microvolts
uint32_t lutSensorValueA0 = 0; // microvolts
uint32_t lutSensorValueA1 = 0; // microvolts

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

void taskLUT(){
    lutA0(movAverageA0);
    lutA1(movAverageA1);
}

/* uint32_t pinVoltageA0(uint16_t x){
    for(uint8_t i=0; i<DIM0-1; i++){
        if(x>=refMesurementA0[i]){
            return (refPinVoltageA0[i] + (refPinVoltageA0[i+1]-refPinVoltageA0[i])*(x-refMesurementA0[i])/(refMesurementA0[i+1]-refMesurementA0[i]));
        }
    }
    return refPinVoltageA0[DIM0];
} */

void lutA0(uint16_t x){
    if(lutIndexA0>DIM0-2){
        lutIndexA0 = DIM0-2;
    } else if (lutIndexA0<0){
        lutIndexA0 = 0;
    }

    if(x>=refMesurementA0[lutIndexA0+1]){
        lutIndexA0++;
        lutPinVoltageA0 = refPinVoltageA0[DIM0-1];
        lutSensorValueA0 = refSensorValueA0[DIM0-1];
    }
    else if(x<refMesurementA0[lutIndexA0]){
        lutIndexA0--;
        lutPinVoltageA0 = refPinVoltageA0[lutIndexA0];
        lutSensorValueA0 = refSensorValueA0[lutIndexA0];
    } 

    lutPinVoltageA0 = (refPinVoltageA0[lutIndexA0] + (refPinVoltageA0[lutIndexA0+1]-refPinVoltageA0[lutIndexA0])*(x-refMesurementA0[lutIndexA0])/(refMesurementA0[lutIndexA0+1]-refMesurementA0[lutIndexA0]));
    lutSensorValueA0 = (refSensorValueA0[lutIndexA0] + (refSensorValueA0[lutIndexA0+1]-refSensorValueA0[lutIndexA0])*(x-refMesurementA0[lutIndexA0])/(refMesurementA0[lutIndexA0+1]-refMesurementA0[lutIndexA0]));
}

void lutA1(uint16_t x){
    if(lutIndexA1>DIM1-2){
        lutIndexA1 = DIM1-2;
    } else if (lutIndexA1<0){
        lutIndexA1 = 0;
    }

    if(x>=refMesurementA1[lutIndexA1+1]){
        lutIndexA1++;
        lutPinVoltageA1 = refPinVoltageA1[DIM1-1];
        lutSensorValueA1 = refSensorValueA1[DIM1-1];
    }
    else if(x<refMesurementA1[lutIndexA1]){
        lutIndexA1--;
        lutPinVoltageA1 = refPinVoltageA1[lutIndexA1];
        lutSensorValueA1 = refSensorValueA1[lutIndexA1];
    } 

    lutPinVoltageA1 = (refPinVoltageA1[lutIndexA1] + (refPinVoltageA1[lutIndexA1+1]-refPinVoltageA1[lutIndexA1])*(x-refMesurementA1[lutIndexA1])/(refMesurementA1[lutIndexA1+1]-refMesurementA1[lutIndexA1]));
    lutSensorValueA1 = (refSensorValueA1[lutIndexA1] + (refSensorValueA1[lutIndexA1+1]-refSensorValueA1[lutIndexA1])*(x-refMesurementA1[lutIndexA1])/(refMesurementA1[lutIndexA1+1]-refMesurementA1[lutIndexA1]));
}


// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]