file(GLOB MCU_MNGR_INTERFACE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(mcu-mngr-interface ${MCU_MNGR_INTERFACE_SOURCES})

target_include_directories(mcu-mngr-interface
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(mcu-mngr-interface PRIVATE c_std_99)

target_compile_options(mcu-mngr-interface PRIVATE -Wall -Wextra -Wpedantic)
