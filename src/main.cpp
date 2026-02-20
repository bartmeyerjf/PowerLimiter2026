// ------------------------------------------------------
// Main Project File
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]

// Include
#include <Arduino.h>
#include "pinconfig.h"
#include "adc.h"
#include "lut.h"
#include "validation.h"

// Test sync
// Setup code
void setup() {
  setupADC();
  setupValidation();
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Loop code
void loop() {
  taskADC();
  taskLUT();
  taskValidation();
}