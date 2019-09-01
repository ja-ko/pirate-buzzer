#pragma once

#include "BuzzerCallbacks.h"
#include "events.h"

#define ESP32
#include "esp32-hal.h"
#include "FastLED.h"

#define TRIGGER_LEDS 43
#define NUM_LEDS 55
#define DATA_PIN 4 
#define LED_TYPE    WS2812B

#define COLORSPACE CRGB

class LEDManager: public BuzzerCallbacks {
public:
    void animate_internal();

protected:
    LEDManager();
    COLORSPACE* base_leds = &leds[0];
    COLORSPACE* base_leds_end = &leds[TRIGGER_LEDS];
    uint8_t base_leds_count = TRIGGER_LEDS;
    COLORSPACE* trigger_leds = &leds[TRIGGER_LEDS];
    COLORSPACE* trigger_leds_end = &leds[NUM_LEDS];
    uint8_t trigger_leds_count = NUM_LEDS - TRIGGER_LEDS;

    virtual void animate(int64_t frameTime) {};

private:
    COLORSPACE leds[NUM_LEDS];
    int64_t last_frame = 0;
};

LEDManager* ledmanager_create();
void ledmanager_setup();

