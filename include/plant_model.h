// ------------------------------------------------------
// Model plant for control
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef plant_model_h
#define plant_model_h
// [====================================================]

#include <Arduino.h>
#include "pinconfig.h"
#include "pwm_out.h"

// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]

volatile uint32_t t0 = 0;
#define t1 8000000
#define t2 8200000
#define t3 4000000
#define dutyStart 1336 // 8.16%
#define dutyFinal 2128 // 12.98%
volatile uint32_t dutyOutput = 0;
volatile uint32_t dutyPrevious = 0;

void taskModel(){};
void ramp(){};
void step(){};

void taskModel(){
  if(t0 = 0){
    t0 = micros();
  }

  dutyPrevious = dutyOutput;
  step();
  if(dutyOutput != dutyPrevious){
    setPWMOutput(dutyOutput);
  }

}

void ramp(){
  if(micros() > t2 + t3 + t0){
    dutyOutput = (dutyStart);
  } else if(micros() > t2 + t0){
    dutyOutput = (dutyFinal);
  } else if(micros() < t1 + t0){
    dutyOutput = (dutyStart);
  } else{
    dutyOutput =(dutyStart+(micros()-t1-t0)*(dutyFinal-dutyStart)/(t2-t1));
  }

}

void step(){
  if(micros() > 3*t3 + t1 + t0){
    dutyOutput = (dutyStart);
  } else if(micros() > 2*t3 + t1 + t0){
    dutyOutput = (dutyFinal);
  } else if(micros() > t3 + t1 + t0){
    dutyOutput = (dutyStart + (dutyFinal-dutyStart)*3/4);
  } else if(micros() > t1 + t0){
    dutyOutput = (dutyStart + (dutyFinal-dutyStart)/4);
  } else{
    dutyOutput = (dutyStart);
  }

}


// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]