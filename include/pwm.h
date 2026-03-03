// ------------------------------------------------------
// PWM Reading Library
// ------------------------------------------------------

// [====================================================]
// [                    HEADER (.h)                     ]
// [====================================================]
// Multiple inclusions lock
#ifndef pwm_h
#define pwm_h
// [====================================================]

#include <Arduino.h>
#include "pinconfig.h"
#include "driver/rmt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/ringbuf.h"

const rmt_channel_t RMT_RX_CHANNEL = RMT_CHANNEL_0;

volatile uint32_t high_duration = 0;
volatile uint32_t low_duration = 0;
volatile uint32_t period = 0;
volatile float duty = 0;

void setupPWM();
void taskPWM();

void setupPWM(){
    rmt_config_t rmt_rx;
    rmt_rx.channel = RMT_RX_CHANNEL;
    rmt_rx.gpio_num = (gpio_num_t)PIN_PWM_IN;
    rmt_rx.clk_div = 80;          // 1 tick = 1 microsecond
    rmt_rx.mem_block_num = 1;     // Use 1 block of RMT RAM
    rmt_rx.rmt_mode = RMT_MODE_RX;

    // Setup RX specific parameters
    rmt_rx.rx_config.idle_threshold = 30000; // 30ms (longer than 13ms period)
    rmt_rx.rx_config.filter_en = true;
    rmt_rx.rx_config.filter_ticks_thresh = 10; // Ignore noise < 10us   
    // Install Driver
    rmt_config(&rmt_rx);
    rmt_driver_install(RMT_RX_CHANNEL, 1000, 0); // 1000 byte ring buffer
}


// [====================================================]
// [               IMPLEMENTATION (.c)                  ]
// [====================================================]

void taskPWM(){

    size_t rx_size = 0;

    // Create a variable to hold the handle
    RingbufHandle_t my_rb_handle;
    // Pass RMT_RX_CHANNEL and the address (&) of handle variable
    esp_err_t err = rmt_get_ringbuf_handle(RMT_RX_CHANNEL, &my_rb_handle);

    if (err != ESP_OK) {
        // Handle error: in case the RMT driver wasn't installed correctly
        return; 
    }

    // Receive pulses from the RMT ring buffer
    void *items = xRingbufferReceive(my_rb_handle, &rx_size, pdMS_TO_TICKS(100));

    if (items != NULL) {
      rmt_item32_t* rmt_data = (rmt_item32_t*)items;
    
      // The RMT records a series of high/low pulses. 
      // We look at the first two items to get one full cycle.
      for (int i = 0; i < (rx_size / sizeof(rmt_item32_t)); i++) {
        if (rmt_data[i].level0 == 1) {
          high_duration = rmt_data[i].duration0;
        } else {
          low_duration = rmt_data[i].duration0;
        }
      } 

      // Calculate Results
      period = high_duration + low_duration;
      if (period > 0) {
        duty = (high_duration / (float)period);   
        // Serial.printf("Duty: %.2f%% | High: %d us | Low: %d us\n", duty, high_duration, low_duration);
      } 
      
      // Clean up the ring buffer memory using the handle variable
      vRingbufferReturnItem(my_rb_handle, items);
    }
}

// [====================================================]
// Close multiple inclusions lock
#endif  
// [====================================================]