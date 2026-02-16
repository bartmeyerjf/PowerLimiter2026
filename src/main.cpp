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

// Setup code
void setup() {
  setupADC();
  if(indexPosition == 0){
    setupValidation();
  }
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