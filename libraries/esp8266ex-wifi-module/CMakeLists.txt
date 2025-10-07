file(GLOB ESP8266EX_WIFI_MODULE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(esp8266ex-wifi-module ${ESP8266EX_WIFI_MODULE_SOURCES})

target_include_directories(esp8266ex-wifi-module
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(esp8266ex-wifi-module PRIVATE c_std_99)

target_compile_options(esp8266ex-wifi-module PRIVATE -Wall -Wextra -Wpedantic)
