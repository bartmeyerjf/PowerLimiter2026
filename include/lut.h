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

#define DIM0 33
#define DIM1 33

void lutA0();
void lutA1();
void taskLUT();

const uint16_t refMesurementA0[DIM0] = {0, 128, 256, 384, 512, 640, 768, 896, 1024, 1152, 1280, 1408, 1536, 1664, 1792, 1920, 2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968, 4096};  // 12 bit value
const uint16_t refMesurementA1[DIM1] = {0, 128, 256, 384, 512, 640, 768, 896, 1024, 1152, 1280, 1408, 1536, 1664, 1792, 1920, 2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968, 4096};  // 12 bit value
const uint32_t refPinVoltageA0[DIM0] = {0, 94, 188, 284, 378, 472, 566, 660, 756, 850, 943, 1035, 1127, 1218, 1309, 1400, 1492, 1582, 1673, 1765, 1857, 1948, 2020, 2110, 2200, 2290, 2380, 2460, 2540, 2620, 2690, 2760, 2840};  // milivolts
const uint32_t refPinVoltageA1[DIM1] = {0, 94, 188, 284, 378, 472, 566, 660, 756, 850, 943, 1035, 1127, 1218, 1309, 1400, 1492, 1582, 1673, 1765, 1857, 1948, 2020, 2110, 2200, 2290, 2380, 2460, 2540, 2620, 2690, 2760, 2840};  // milivolts
const uint32_t refSensorValueA0[DIM0] = {0, 890, 1790, 2680, 3580, 4470, 5360, 6260, 7170, 8050, 8940, 9810, 10680, 11540, 12400, 13270, 14140, 14990, 15850, 16720, 17590, 18450, 19320, 20200, 21100, 21900, 22800, 23600, 24400, 25100, 25800, 26500, 27100}; // milivolts
const uint32_t refSensorValueA1[DIM1] = {}; // microvolts

uint8_t lutIndexA0 = DIM0-1;
uint8_t lutIndexA1 = DIM1-1;
uint32_t lutPinVoltageA0 = refPinVoltageA0[DIM0-1];  // milivolts
uint32_t lutPinVoltageA1 = refPinVoltageA0[DIM1-1];  // milivolts
uint32_t lutSensorValueA0 = 0; // 
uint32_t lutSensorValueA1 = 0; // 

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

void taskLUT(){
    lutA0();
    //lutA1(movAverageA1);
}

/* uint32_t pinVoltageA0(uint16_t movAverageA0){
    for(uint8_t i=0; i<DIM0-1; i++){
        if(movAverageA0>=refMesurementA0[i]){
            return (refPinVoltageA0[i] + (refPinVoltageA0[i+1]-refPinVoltageA0[i])*(movAverageA0-refMesurementA0[i])/(refMesurementA0[i+1]-refMesurementA0[i]));
        }
    }
    return refPinVoltageA0[DIM0];
} */

void lutA0(){
    // check if values are out of bound
    if(lutIndexA0>DIM0-2){
        lutIndexA0 = DIM0-2;
    } else if (lutIndexA0<0){
        lutIndexA0 = 0;
    }

    if(movAverageA0>=refMesurementA0[lutIndexA0+1]){
        lutIndexA0++;
        lutPinVoltageA0 = refPinVoltageA0[lutIndexA0+1];
        lutSensorValueA0 = refSensorValueA0[lutIndexA0+1];
    }
    else if(movAverageA0<refMesurementA0[lutIndexA0]){
        lutIndexA0--;
        lutPinVoltageA0 = refPinVoltageA0[lutIndexA0];
        lutSensorValueA0 = refSensorValueA0[lutIndexA0];
    } else{
        lutPinVoltageA0 = (refPinVoltageA0[lutIndexA0] + (refPinVoltageA0[lutIndexA0+1]-refPinVoltageA0[lutIndexA0])*(movAverageA0-refMesurementA0[lutIndexA0])/(refMesurementA0[lutIndexA0+1]-refMesurementA0[lutIndexA0]));
        lutSensorValueA0 = (refSensorValueA0[lutIndexA0] + (refSensorValueA0[lutIndexA0+1]-refSensorValueA0[lutIndexA0])*(movAverageA0-refMesurementA0[lutIndexA0])/(refMesurementA0[lutIndexA0+1]-refMesurementA0[lutIndexA0]));
    }
}

void lutA1(){
    if(lutIndexA1>DIM1-2){
        lutIndexA1 = DIM1-2;
    } else if (lutIndexA1<0){
        lutIndexA1 = 0;
    }

    if(movAverageA1>=refMesurementA1[lutIndexA1+1]){
        lutIndexA1++;
        lutPinVoltageA1 = refPinVoltageA1[lutIndexA1+1];
        lutSensorValueA1 = refSensorValueA1[lutIndexA1+1];
    }
    else if(movAverageA1<refMesurementA1[lutIndexA1]){
        lutIndexA1--;
        lutPinVoltageA1 = refPinVoltageA1[lutIndexA1];
        lutSensorValueA1 = refSensorValueA1[lutIndexA1];
    } else{
        lutPinVoltageA1 = (refPinVoltageA1[lutIndexA1] + (refPinVoltageA1[lutIndexA1+1]-refPinVoltageA1[lutIndexA1])*(movAverageA1-refMesurementA1[lutIndexA1])/(refMesurementA1[lutIndexA1+1]-refMesurementA1[lutIndexA1]));
        lutSensorValueA1 = (refSensorValueA1[lutIndexA1] + (refSensorValueA1[lutIndexA1+1]-refSensorValueA1[lutIndexA1])*(movAverageA1-refMesurementA1[lutIndexA1])/(refMesurementA1[lutIndexA1+1]-refMesurementA1[lutIndexA1]));
    }
}


// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]