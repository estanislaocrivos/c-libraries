file(GLOB SPI_INTERFACE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(spi-interface ${SPI_INTERFACE_SOURCES})

target_include_directories(spi-interface
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(spi-interface PRIVATE c_std_99)

target_compile_options(spi-interface PRIVATE -Wall -Wextra -Wpedantic)
