file(GLOB UIP_STACK_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(uip ${UIP_STACK_SOURCES})

target_include_directories(uip
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(uip PRIVATE c_std_99)

target_compile_options(uip PRIVATE -Wall -Wextra -Wpedantic)
