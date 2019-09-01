#include "LEDManager.h"

class SimpleLEDManager: public LEDManager {
public:
    SimpleLEDManager(): LEDManager() {
        fill_solid(base_leds, base_leds_count, CRGB::Orange);
    }

    void onBuzzerDown() {
        fill_solid(trigger_leds, trigger_leds_count, CRGB::Green);
    }

    void onBuzzerUp() {
        fill_solid(trigger_leds, trigger_leds_count, CRGB::Red);
    }
};

LEDManager* ledmanager_create() {
    return new SimpleLEDManager();
}
