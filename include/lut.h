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

#define DIM0 37
#define DIM1 16

void lutA0(uint16_t x);
void lutA1(uint16_t x);
void taskLUT();

const uint16_t refMesurementA0[DIM0] = {4, 8, 16, 32, 64, 128, 256, 384, 512, 640, 768, 896, 1024, 1152, 1280, 1408, 1536, 1664, 1792, 1920, 2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968, 4096};  // 12 bit value
const uint16_t refMesurementA1[DIM1] = {};  // 12 bit value
const uint32_t refPinVoltageA0[DIM0] = {0, 0, 0, 30, 40, 96, 192, 285, 382, 480, 575, 670, 770, 860, 960, 1050, 1145, 1235, 1330, 1420, 1515, 1600, 1695, 1794, 1880, 1975, 2065, 2156, 2250, 2348, 2428, 2515, 2590, 2675, 2755, 2822, 2895};  // milivolts
const uint32_t refPinVoltageA1[DIM1] = {};  // microvolts
const uint32_t refSensorValueA0[DIM0] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // milivolts
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
    //lutA1(movAverageA1);
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
    // check if values are out of bound
    if(lutIndexA0>DIM0-2){
        lutIndexA0 = DIM0-2;
    } else if (lutIndexA0<0){
        lutIndexA0 = 0;
    }

    if(x>=refMesurementA0[lutIndexA0+1]){
        lutIndexA0++;
        lutPinVoltageA0 = refPinVoltageA0[lutIndexA0+1];
        lutSensorValueA0 = refSensorValueA0[lutIndexA0+1];
    }
    else if(x<refMesurementA0[lutIndexA0]){
        lutIndexA0--;
        lutPinVoltageA0 = refPinVoltageA0[lutIndexA0];
        lutSensorValueA0 = refSensorValueA0[lutIndexA0];
    } else{
        lutPinVoltageA0 = (refPinVoltageA0[lutIndexA0] + (refPinVoltageA0[lutIndexA0+1]-refPinVoltageA0[lutIndexA0])*(x-refMesurementA0[lutIndexA0])/(refMesurementA0[lutIndexA0+1]-refMesurementA0[lutIndexA0]));
        lutSensorValueA0 = (refSensorValueA0[lutIndexA0] + (refSensorValueA0[lutIndexA0+1]-refSensorValueA0[lutIndexA0])*(x-refMesurementA0[lutIndexA0])/(refMesurementA0[lutIndexA0+1]-refMesurementA0[lutIndexA0]));
    }
}

void lutA1(uint16_t x){
    if(lutIndexA1>DIM1-2){
        lutIndexA1 = DIM1-2;
    } else if (lutIndexA1<0){
        lutIndexA1 = 0;
    }

    if(x>=refMesurementA1[lutIndexA1+1]){
        lutIndexA1++;
        lutPinVoltageA1 = refPinVoltageA1[lutIndexA1+1];
        lutSensorValueA1 = refSensorValueA1[lutIndexA1+1];
    }
    else if(x<refMesurementA1[lutIndexA1]){
        lutIndexA1--;
        lutPinVoltageA1 = refPinVoltageA1[lutIndexA1];
        lutSensorValueA1 = refSensorValueA1[lutIndexA1];
    } else{
        lutPinVoltageA1 = (refPinVoltageA1[lutIndexA1] + (refPinVoltageA1[lutIndexA1+1]-refPinVoltageA1[lutIndexA1])*(x-refMesurementA1[lutIndexA1])/(refMesurementA1[lutIndexA1+1]-refMesurementA1[lutIndexA1]));
        lutSensorValueA1 = (refSensorValueA1[lutIndexA1] + (refSensorValueA1[lutIndexA1+1]-refSensorValueA1[lutIndexA1])*(x-refMesurementA1[lutIndexA1])/(refMesurementA1[lutIndexA1+1]-refMesurementA1[lutIndexA1]));
    }
}


// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]