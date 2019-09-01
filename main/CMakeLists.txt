set(COMPONENT_SRCS "main.cpp" "BluetoothManager.cpp" "LEDManager.cpp" "SimpleLEDManager.cpp")

set(COMPONENT_ADD_INCLUDEDIRS "")

set(COMPONENT_REQUIRES
    "esp32-snippets"
    "FastLED"
    "esp_event"
)

register_component()
