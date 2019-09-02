#include "LEDManager.h"

#include "esp_log.h"

class SimpleLEDManager: public LEDManager {
public:
    SimpleLEDManager(): LEDManager() {
        fill_solid(base_leds, base_leds_count, CRGB::BlueViolet);
        onBuzzerUp();

        //setup palette
        fill_solid(palette, 16, CRGB::Black);
        palette[0] = CRGB::White;
        palette[8] = CRGB::White;
    }

    void onBuzzerDown() override {
        fill_solid(trigger_leds, trigger_leds_count, CRGB::Green);
    }

    void onBuzzerUp() override {
        fill_solid(trigger_leds, trigger_leds_count, CRGB::Red);
    }

    void onClientConnected() override {
        fill_solid(trigger_leds, trigger_leds_count, CRGB::Blue);
        reset_trigger = elapsed + 500;
    }

    void onClientDisconnected() override {
        fill_solid(trigger_leds, trigger_leds_count, CRGB::Yellow);
        reset_trigger = elapsed + 500;
    }

    void onExternalEvent(buzzer_remote_event_data_t* data) override {
        palette_until = elapsed + 15000;
    };

    void animate(int64_t frameTime) override {
        static uint8_t index = 0;
        elapsed += frameTime;

        if(reset_trigger != 0 && reset_trigger < elapsed) {
            reset_trigger = 0;
            onBuzzerUp();
        }
        if(palette_until > 0) {
            if(palette_until > elapsed) {
                for( int i = 0; i < base_leds_count; i++) {
                    base_leds[i] = ColorFromPalette(palette, index);
                    index += 3;
                }
            } else {
                fill_solid(base_leds, base_leds_count, CRGB::BlueViolet);
                palette_until = 0;
            }
        } 
    }

private:
    int64_t elapsed = 0;
    int64_t reset_trigger = 0;
    int64_t palette_until = 0;
    CRGBPalette16 palette;
};

LEDManager* ledmanager_create() {
    return new SimpleLEDManager();
}
