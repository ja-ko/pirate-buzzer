# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES
  "console"
  "fatfs"
  "json"
  "mdns"
  "nvs_flash"
  "bt"
)
set(COMPONENT_PRIV_REQUIRES )

file(GLOB COMPONENT_SRCS
  LIST_DIRECTORIES false
  "esp32-snippets/cpp_utils/*.h"
  "esp32-snippets/cpp_utils/*.cpp"
  "esp32-snippets/cpp_utils/*.c"
  "esp32-snippets/cpp_utils/*.S"
)
set(COMPONENT_ADD_INCLUDEDIRS "esp32-snippets/cpp_utils")

register_component()
