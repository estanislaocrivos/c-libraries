file(GLOB LCD_16_2_DRIVER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(lcd-16-by-2 ${LCD_16_2_DRIVER_SOURCES})

target_include_directories(lcd-16-by-2 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_compile_features(lcd-16-by-2 PRIVATE c_std_99)

target_compile_options(lcd-16-by-2 PRIVATE -Wall -Wextra -Wpedantic)
