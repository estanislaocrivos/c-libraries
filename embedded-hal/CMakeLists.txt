file(GLOB MCU_MNGR_INTERFACE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(embedded-hal ${MCU_MNGR_INTERFACE_SOURCES})

target_include_directories(embedded-hal
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(embedded-hal PRIVATE c_std_99)

target_compile_options(embedded-hal PRIVATE -Wall -Wextra -Wpedantic)
