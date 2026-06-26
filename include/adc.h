// ------------------------------------------------------
// Analog to Digital Converter Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef adc_h
#define adc_h
// [====================================================]

#include <Arduino.h>
#include "pinconfig.h"

//#include "pwm_in.h"

void setupADC();
void adcRead();
void setupAdcContinuous();
void adcContinuousRead();

// ADC buffer (n samples)
#define BUFFER_SIZE 512

volatile uint16_t currentReading = 0;
volatile uint16_t voltageReading = 0;

// ========== ADC Continuous Mode Config ==============

// Define pins array to sample (Only ADC1 pins are supported in continuous mode)
const uint8_t adc_pins[] = { PIN_A0, PIN_A1 };
const uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);

// Number of raw conversions to average per cycle
#define CONVERSIONS_PER_PIN 1

// Flag flipped by the background ISR when new averaged data is ready
volatile bool adc_conversion_done = false;

// Depending on your precise ESP32 core minor version, the struct name 
// might be "adc_continuous_result_t" or the older "adc_continuous_data_t".
// We declare a pointer to hold the results accordingly:
adc_continuous_result_t *adc_results = NULL;

// ISR Function called automatically when DMA completes a buffer cycle
void ARDUINO_ISR_ATTR onAdcComplete() {
    adc_conversion_done = true;
}

void setupAdcContinuous() {

    // Set resolution (9-12 bits, default is 12 bits)
    analogContinuousSetWidth(12);

    // Set input attenuation (Default is ADC_11db, which is standard for ~0-3.1V range)
    analogContinuousSetAtten(ADC_11db);

    // Setup the hardware peripheral
    bool init_success = analogContinuous(
        adc_pins, 
        adc_pins_count, 
        CONVERSIONS_PER_PIN, 
        20000,           // Sample rate (20kHz is standard)
        &onAdcComplete   // ISR callback
    );

    if (init_success) {
        // Start background hardware DMA sampling
        analogContinuousStart();
    }
}


// ====================================================

// Buffers
uint16_t bufferA0_0[BUFFER_SIZE];
uint16_t bufferA0_1[BUFFER_SIZE];
uint16_t bufferA1_0[BUFFER_SIZE];
uint16_t bufferA1_1[BUFFER_SIZE];
uint32_t timeStamp_0[BUFFER_SIZE];
uint32_t timeStamp_1[BUFFER_SIZE];
uint32_t dutyCycle[BUFFER_SIZE];

uint16_t bufferIndex = 0; 
volatile bool activeBuffer = 0; 
volatile bool isBufferReady = false;

void setupADC() {
  // Set attenuation 11dB -> 0-2.8V
  //analogSetAttenuation(ADC_11db);

  setupAdcContinuous();

  // Initialize buffers with zeros
  for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
    bufferA0_0[i] = 0;
    bufferA0_1[i] = 0;
    bufferA1_0[i] = 0;
    bufferA1_1[i] = 0;
    timeStamp_0[i] = 0;
    timeStamp_1[i] = 0;
    dutyCycle[i] = 0;
  }
}

// [====================================================]
// [                 IMPLEMENTATION (.c)                ]
// [====================================================]

void adcContinuousRead() {
  // Retrieve the parsed results (non-blocking call with 0ms timeout)
  if (analogContinuousRead(&adc_results, 0)) {
    voltageReading = adc_results[PIN_A0].avg_read_raw;
    currentReading = adc_results[PIN_A1].avg_read_raw;
  }

    // save in active buffer
  if (activeBuffer == 1) {
    bufferA0_1[bufferIndex] = voltageReading;
    bufferA1_1[bufferIndex] = currentReading;
    timeStamp_1[bufferIndex] = micros();
    //dutyCycle[bufferIndex] = (pwmInDuty);
  } else {
    bufferA0_0[bufferIndex] = voltageReading;
    bufferA1_0[bufferIndex] = currentReading;
    timeStamp_0[bufferIndex] = micros();
    //dutyCycle[bufferIndex] = (pwmInDuty);
  }

  // Update index and flag when buffer is full
  bufferIndex ++;
  if (bufferIndex >= BUFFER_SIZE) {
    bufferIndex = 0;
    activeBuffer = !(activeBuffer);
    isBufferReady = true; // Tell main loop to write to SD
  }
  
}

void adcRead() {
  // Read ADC and 
  voltageReading = analogRead(A0);
  currentReading = analogRead(A1);

  // save in active buffer
  if (activeBuffer == 1) {
    bufferA0_1[bufferIndex] = voltageReading;
    bufferA1_1[bufferIndex] = currentReading;
    //timeStamp_1[bufferIndex] = micros();
    //dutyCycle[bufferIndex] = (pwmInDuty);
  } else {
    bufferA0_0[bufferIndex] = voltageReading;
    bufferA1_0[bufferIndex] = currentReading;
    //timeStamp_0[bufferIndex] = micros();
    //dutyCycle[bufferIndex] = (pwmInDuty);
  }

  // Update index and flag when buffer is full
  bufferIndex ++;
  if (bufferIndex >= BUFFER_SIZE) {
    bufferIndex = 0;
    activeBuffer = !(activeBuffer);
    isBufferReady = true; // Tell main loop to write to SD
  }
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]