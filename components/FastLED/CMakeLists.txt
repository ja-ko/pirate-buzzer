set(COMPONENT_SRCS "FastLED/bitswap.cpp" "FastLED/colorpalettes.cpp" "FastLED/colorutils.cpp" "esp32-hal-gpio.c" "esp32-hal.c" "FastLED/FastLED.cpp" "FastLED/hsv2rgb.cpp" "FastLED/lib8tion.cpp" "FastLED/noise.cpp" "FASTLED/platforms.cpp" "FastLED/power_mgt.cpp" "FastLED/wiring.cpp")
set(COMPONENT_ADD_INCLUDEDIRS "FastLED" "FastLED/lib8tion" "FastLED/platforms/esp/32")
register_component()