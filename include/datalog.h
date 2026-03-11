// ------------------------------------------------------
// Save data in SD card
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef datalog_h
#define datalog_h
// [====================================================]

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "pinconfig.h"
#include "adc.h"

void setupDataLog();
void taskDataLog();
void writeSD();
void writeSerial();

File dataFile;

void setupDataLog(){

    if (!SD.begin(PIN_SD_CS)) {
      Serial.println("SD Initialization Failed!");
      while (1); // Stop
    }
    // Open file
    dataFile = SD.open("/session.csv", FILE_WRITE);
    if (!dataFile) {
      Serial.println("File Open Failed!");
      while (1);
    }
    dataFile.println("dyty cycle, time (us), A0, A1"); // Header

}

// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]

void taskDataLog(){
    if (isBufferReady) {
        // Pointer to the buffer that finished filling
        uint16_t* b0 = (activeBuffer == 1) ? bufferA0_1 : bufferA0_0;
        uint16_t* b1 = (activeBuffer == 1) ? bufferA1_1 : bufferA1_0;
        uint32_t* timeStamp = (activeBuffer == 1) ? timeStamp_1 : timeStamp_0;
        uint32_t* dutyLog = dutyCycle;

        for (int i = 0; i < BUFFER_SIZE; i++) {
            // Write to SD
            dataFile.print(100*(float)dutyLog[i]/16383);
            dataFile.print(",");
            dataFile.print(timeStamp[i]);
            dataFile.print(",");
            dataFile.print(b0[i]);
            dataFile.print(",");
            dataFile.println(b1[i]);

            // Write to Serial
            Serial.print(100*(float)dutyLog[i]/16383);
            Serial.print(",");
            Serial.print(timeStamp[i]);
            Serial.print(",");
            Serial.print(b0[i]);
            Serial.print(",");
            Serial.println(b1[i]);
        }
        dataFile.flush(); // Ensure data is saved
        isBufferReady = false; // Reset flag
    }
}

void writeSD(){

}

void writeSerial(){

}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]