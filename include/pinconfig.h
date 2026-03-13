// ------------------------------------------------------
// ESP32-C3 Pin Setup Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef pinconfig_h
#define pinconfig_h
// [====================================================]

// ADC Pins
#define PIN_ADC_READ_VOLTAGE A0
#define PIN_ADC_READ_CURRENT A1

// SD card pins
#define PIN_SD_MISO 5
#define PIN_SD_MOSI 6
#define PIN_SD_SCK 4
#define PIN_SD_CS 7

// PWM input pin
#define PIN_PWM_IN GPIO_NUM_8

// PWM input pin
#define PIN_PWM_OUT 10

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]