// ------------------------------------------------------
// Convert mesured values into real values
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef datalfit_h
#define datalfit_h
// [====================================================]

#include <Arduino.h>

#define voltageSlope 6.82
#define voltageZero 119.9
#define currentSlope -10.98
#define currentZero 38797

float voltageFit(uint16_t readingValue);
float currentFit(uint16_t readingValue);

// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]

// Linar regression is used to convert mesurements into real values

// reading to mV
float voltageFit(uint16_t readingValue){
    return(voltageSlope*readingValue+voltageZero);
}

// reading to mA
float currentFit(uint16_t readingValue){
    return(currentSlope*readingValue+currentZero);
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]