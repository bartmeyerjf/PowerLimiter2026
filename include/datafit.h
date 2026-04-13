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

uint16_t voltageFit(uint16_t readingValue){};
uint16_t currentFit(uint16_t readingValue){};

// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]

// Linar regression is used to convert mesurements into real values

uint16_t voltageFit(uint16_t readingValue){
    return(voltageSlope*readingValue+voltageZero);
}

uint16_t currentFit(uint16_t readingValue){
    return(currentSlope*readingValue+currentZero);
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]