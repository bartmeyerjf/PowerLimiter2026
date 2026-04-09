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

#define voltageSlope 6.820
#define voltageZero 119.9
#define currentSlope -10.98
#define currentZero 38797

float voltage(uint16_t readingValue){};
float current(uint16_t readingValue){};

// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]

float voltage(uint16_t readingValue){
    return(voltageSlope*(float)readingValue+voltageZero);
}

float current(uint16_t readingValue){
    return(currentSlope*(float)readingValue+currentZero);
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]