#include <Arduino.h>

// For ESP32-C3: Pin A0 is GPIO 0, Pin A1 is GPIO 1
#define PIN_A0 0
#define PIN_A1 1

// Define pins array to sample (Only ADC1 pins are supported in continuous mode)
const uint8_t adc_pins[] = { PIN_A0, PIN_A1 };
const uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);

// Number of raw conversions to average per cycle
#define CONVERSIONS_PER_PIN 5

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

void setupADCC() {

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

void taskADCC() {
    // Check if the background DMA engine has finished a sampling run
    if (adc_conversion_done) {
        adc_conversion_done = false; // Reset the ISR flag

        // Retrieve the parsed results (non-blocking call with 0ms timeout)
        if (analogContinuousRead(&adc_results, 0)) {
            
            // Temporary variables
            uint16_t a0_raw = 0, a1_raw = 0;
            //int a0_mv = 0, a1_mv = 0;

            // Iterate through the results array matching our configured pin count
            for (int i = 0; i < adc_pins_count; i++) {
                if (adc_results[i].pin == PIN_A0) {
                    a0_raw = adc_results[i].avg_read_raw;
                    //a0_mv  = adc_results[i].avg_read_mvolts;
                } else if (adc_results[i].pin == PIN_A1) {
                    a1_raw = adc_results[i].avg_read_raw;
                    //a1_mv  = adc_results[i].avg_read_mvolts;
                }
            }

        }
    }

}