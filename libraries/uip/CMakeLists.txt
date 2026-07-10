file(GLOB PICO_NET_STACK_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(pico-net-stack ${PICO_NET_STACK_SOURCES})

target_include_directories(pico-net-stack
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(pico-net-stack PRIVATE c_std_99)

target_compile_options(pico-net-stack PRIVATE -Wall -Wextra -Wpedantic)
