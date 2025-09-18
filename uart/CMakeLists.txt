file(GLOB UART_INTERFACE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(uart ${UART_INTERFACE_SOURCES})

target_include_directories(uart
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(uart PRIVATE c_std_99)

target_compile_options(uart PRIVATE -Wall -Wextra -Wpedantic)
