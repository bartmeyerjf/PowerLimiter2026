// ------------------------------------------------------
// Main Project File
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]

// Include
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>

#include "pinconfig.h"
#include "adc.h"
#include "datalog.h"

#include "pwm_in.h"
#include "pwm_out.h"

//#include "lut.h"
//#include "validation.h"

volatile uint32_t t0 = 0;
#define timeStart 8000000
#define timeFinal 8800000
#define timeSteady 5000000
#define dutyStart 8.16
#define dutyFinal 12.98

volatile float dutyOutput = 0;


void IRAM_ATTR onTimer();

// Timer Interrupt
hw_timer_t *timer = NULL;

// Setup code
void setup() {
  // Start serial monitor
  Serial.begin(460800);
  // Turn off wifi and bluetooth
  WiFi.mode(WIFI_OFF);
  btStop();

  // Timer Setup (1kHz -> 1000 us)
  timer = timerBegin(0, 80, true); // prescaler 80 -> 1000 us
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

  // Setup libraries
  setupADC();
  setupPWMIn();
  setupDataLog();
  setupPWMOut();

  pinMode(21, INPUT);
  setPWMOutput((uint32_t)(8.16*16383/100));
  
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

volatile bool runLoop = 0;

volatile uint16_t prevCurrentReading = 0;
volatile float prevCurrentReadingmV = 0;

// Loop code
void loop() {

/*   if((prevCurrentReading!=currentReading) || (prevCurrentReadingmV != prevCurrentReadingmV)){
    Serial.print((currentReading+prevCurrentReading)/2);
    Serial.print(", ");
    Serial.println((currentReadingmV+prevCurrentReadingmV)/2);
    prevCurrentReading = currentReading;
    prevCurrentReadingmV = currentReadingmV;
  } */

  taskPWMIn();
  taskDataLog();

  if(t0 = 0){
    t0 = micros();
  }

  if(micros() > timeFinal + timeSteady*2 + t0){
    dutyOutput = (dutyStart);
  } else if(micros() > timeFinal + timeSteady + t0){
    dutyOutput = (dutyStart + dutyFinal)/2;
  } else if(micros() > timeFinal + t0){
    dutyOutput = (dutyFinal);
  } else if(micros() < timeStart + t0){
    dutyOutput = (dutyStart);
  } else{
    dutyOutput =(dutyStart+(micros()-timeStart-t0)*(dutyFinal-dutyStart)/(timeFinal-timeStart));
  }


  setPWMOutput((uint32_t)(dutyOutput*16383/100));
  
 /*  runLoop = digitalRead(21);
  if(true) {
    // Set pwm out to be the same as pwm in
    if((micros() < t0 + t1)){
      setPWMOutput((uint32_t)(8.16*16383/100));
    } else if((micros() < (t0 + 2*t1))){
      setPWMOutput((uint32_t)(8.64*16383/100));
    } else if((micros() < (t0 + 3*t1))){
      setPWMOutput((uint32_t)(9.13*16383/100));
    } else if((micros() < (t0 + 4*t1))){
      setPWMOutput((uint32_t)(9.61*16383/100));
    } else if((micros() < (t0 + 5*t1))){
      setPWMOutput((uint32_t)(10.09*16383/100));
    } else if((micros() < (t0 + 6*t1))){
      setPWMOutput((uint32_t)(10.58*16383/100));
    } else if((micros() < (t0 + 7*t1))){
      setPWMOutput((uint32_t)(11.06*16383/100));
    } else if((micros() < (t0 + 8*t1))){
      setPWMOutput((uint32_t)(11.54*16383/100));
    } else if((micros() < (t0 + 9*t1))){
      setPWMOutput((uint32_t)(12.02*16383/100));
    } else if((micros() < (t0 + 10*t1))){
      setPWMOutput((uint32_t)(12.51*16383/100));
    } else if((micros() < (t0 + 17*t2))){
      setPWMOutput((uint32_t)(12.99*16383/100));
    } else if((micros() < (t0 + 20*t2))){
      setPWMOutput((uint32_t)(10.58*16383/100));
    } else{
      setPWMOutput((uint32_t)(8.16*16383/100));
    } 
  } */

  //setPWMOutput((uint32_t)(8.16*16383/100)*);
  
}

// 1 kHz interrupt code
void IRAM_ATTR onTimer(){
  adcRead();
  
} 