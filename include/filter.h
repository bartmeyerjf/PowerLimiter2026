// ------------------------------------------------------
// Library for digital signal filter
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef filter_h
#define filter_h
// [====================================================]

#include <Arduino.h>

// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]
// #define SHIFT 1 // alpha = 0.5 -> ~1 sample delay
#define SHIFT 2 // alpha = 0.25 -> ~3 samples delay
long filteredValue = 0;
bool initialized = false;

int filter(int reading);

int filter(int reading){
    // If it's the first reading, jump-start the filter 
    // so it doesn't "climb" from zero (prevents initial delay)
    if (!initialized) {
        filteredValue = (long)reading << SHIFT;
        initialized = true;
    }

    // Fixed-point EMA Formula: 
    // NewSum = OldSum - Average + NewReading
    filteredValue = filteredValue - (filteredValue >> SHIFT) + reading;

    // Return the scaled-down result
    return (filteredValue >> SHIFT);

}


// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]