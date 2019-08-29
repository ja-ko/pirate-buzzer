file(GLOB COMPONENT_SRCS
  LIST_DIRECTORIES false
  "*.h"
  "*.cpp"
)

set(COMPONENT_ADD_INCLUDEDIRS "")

set(COMPONENT_REQUIRES
    "esp32-snippets"
    "FastLED"
)

register_component()
