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

volatile uint32_t t0 = 8000000;
#define t1 8000000
#define t2 8200000
#define t3 4000000
#define dutyStart 1336 // 8.16%
#define dutyFinal 2128 // 12.98%
#define maxPowerSetpoint 500 // max value for power setpoint in W
volatile uint32_t rcDutySetpoint = 0;  // remote controller duty setpoint in 16 bit value
volatile uint32_t rcDutySetpointPrevious = 0;
volatile uint32_t rcPowerSetpoint = 0; // remote controller duty setpoint in Watts

void taskModel();
void ramp();
void step();

void taskModel(){
  if(t0 = 0){
    t0 = micros();
  }

  rcDutySetpointPrevious = rcDutySetpoint;
  step();
  //ramp();
  if(rcDutySetpoint != rcDutySetpointPrevious){
    setPWMOutput(rcDutySetpoint);
  }

  rcPowerSetpoint = dutyStart + rcDutySetpoint*maxPowerSetpoint/dutyFinal;

}

void ramp(){
  if(micros() > t2 + t3 + t0){
    rcDutySetpoint = (dutyStart);
  } else if(micros() > t2 + t0){
    rcDutySetpoint = (dutyFinal);
  } else if(micros() < t1 + t0){
    rcDutySetpoint = (dutyStart);
  } else{
    rcDutySetpoint =(dutyStart+(micros()-t1-t0)*(dutyFinal-dutyStart)/(t2-t1));
  }

}

void step(){
  if(micros() > 3*t3 + t1 + t0){
    rcDutySetpoint = (dutyStart);
  } else if(micros() > 2*t3 + t1 + t0){
    rcDutySetpoint = (dutyFinal);
  } else if(micros() > t3 + t1 + t0){
    rcDutySetpoint = (dutyStart + (dutyFinal-dutyStart)*3/4);
  } else if(micros() > t1 + t0){
    rcDutySetpoint = (dutyStart + (dutyFinal-dutyStart)/4);
  } else{
    rcDutySetpoint = (dutyStart);
  }

}


// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]