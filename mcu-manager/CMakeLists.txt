file(GLOB MCU_MNGR_INTERFACE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(mcu-manager ${MCU_MNGR_INTERFACE_SOURCES})

target_include_directories(mcu-manager
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(mcu-manager PRIVATE c_std_99)

target_compile_options(mcu-manager PRIVATE -Wall -Wextra -Wpedantic)
