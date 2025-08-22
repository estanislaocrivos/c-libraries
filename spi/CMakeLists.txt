file(GLOB SPI_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(spi ${SPI_SOURCES})

target_include_directories(spi
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(spi PRIVATE c_std_99)

target_compile_options(spi PRIVATE -Wall -Wextra -Wpedantic)
