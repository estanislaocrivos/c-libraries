file(GLOB LCD_16_2_DRIVER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(lcd-16-2-driver ${LCD_16_2_DRIVER_SOURCES})

target_include_directories(lcd-16-2-driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(lcd-16-2-driver PRIVATE c_std_99)

target_compile_options(lcd-16-2-driver PRIVATE -Wall -Wextra -Wpedantic)
