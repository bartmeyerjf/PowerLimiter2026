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
#define t1 1000000
#define t2 7000000
#define t3 10000000
#define t4 10000000
#define t5 10000000
#define t6 10000000
#define t7 10000000
#define t8 10000000


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
  setupPWMOut();
  setupDataLog();

  pinMode(21, INPUT);
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

volatile bool runLoop = 0;

// Loop code
void loop() {
  // Set pwm out to be the same as pwm in
  //setPWMOutput(((uint32_t)(8.16*16383/100)));
  taskPWMIn();
  taskDataLog();
  
  //setPWMOutput((uint32_t)(8.16*16383/100));
  
  runLoop = digitalRead(21);
  if(runLoop)
  {
    // Set pwm out to be the same as pwm in
    if(micros() < t0 + t1){
      setPWMOutput((uint32_t)(8.16*16383/100));
    } else{
      setPWMOutput((uint32_t)(8.88*16383/100));
    } 
  } else{
    t0 = micros();
  }
  
}

// 1 kHz interrupt code
void IRAM_ATTR onTimer(){
  adcRead();
  
} 