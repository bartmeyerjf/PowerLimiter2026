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

// Function declarations
int myFunction(int, int);

// Setup code
void setup() {
  Serial.begin(115200);  // begin serial monitor
  
  setupADC();



  int result = myFunction(2, 3);
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

// Loop code
void loop() {
  //Serial.println("Working");
  taskADC();
}

// Function definitions
int myFunction(int x, int y) {
  return x + y;
}