file(GLOB LCD_16_2_DRIVER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(hd44780 ${LCD_16_2_DRIVER_SOURCES})

target_include_directories(hd44780 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_compile_features(hd44780 PRIVATE c_std_99)

target_compile_options(hd44780 PRIVATE -Wall -Wextra -Wpedantic)
