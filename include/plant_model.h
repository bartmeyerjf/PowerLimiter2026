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
#define t2 8500000
#define t3 4000000
#define timeStart 10000000
#define timeSignal 11000000
#define timeEnd 20000000
#define dutyStart 1336 // 8.16%
#define dutyFinal 2128 // 12.98%
#define maxPowerSetpoint 500 // max value for power setpoint in W
volatile uint16_t rcDutySetpoint = 0;  // remote controller duty setpoint in 16 bit value
volatile float rcPowerSetpoint = 0; // remote controller duty setpoint in Watts

void taskModel();
void ramp();
void step();
float dutyValueToPercentage(uint32_t dutyValue);
uint16_t dutyPercentageToValue(float dutyPercentage);

void taskModel(){
  if(t0 == 0){
    t0 = micros();
  }
  
  //step();
  rampDuty();
  //rcPowerSetpoint = maxPowerSetpoint*dutyValueToPercentage(rcDutySetpoint);

}

void rampOld(){
  if(micros() > t2 + t3 + 6000000 + t0){
    rcDutySetpoint = (dutyStart);
  } else if(micros() > t2 + t0){
    rcDutySetpoint = (dutyFinal);
  } else if(micros() < t1 + t0){
    rcDutySetpoint = (dutyStart);
  } else{
    rcDutySetpoint =(dutyStart+(micros()-t1-t0)*(dutyFinal-dutyStart)/(t2-t1));
  }
}


void ramp(){
  if((micros() < timeStart + t0) || (micros() > timeEnd + t0) ){
    // set output to zero at beguining and end
    //rcDutySetpoint = (dutyStart);
    rcPowerSetpoint = 0;
  } else if(micros() < timeSignal + t0){
    // ramp
    rcPowerSetpoint =((micros()-timeStart-t0)*(maxPowerSetpoint)/(timeSignal-timeStart));
  } else{
    rcPowerSetpoint = maxPowerSetpoint;
  }
}

void rampDuty(){
  if((micros() < timeStart + t0) || (micros() > timeEnd + t0) ){
    // set output to zero at beguining and end
    rcDutySetpoint = (dutyStart);
  } else if(micros() < timeSignal + t0){
    // ramp
    rcDutySetpoint =((micros()-timeStart-t0)*(dutyFinal - dutyStart)/(timeSignal-timeStart) + dutyStart);
  } else{
    rcDutySetpoint = dutyFinal;
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

float dutyValueToPercentage(uint16_t dutyValue){
  return (dutyValue-1336)/(2128-1336);
}

uint16_t dutyPercentageToValue(float dutyPercentage){
  return dutyPercentage*(2128-1336)+1336;
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]