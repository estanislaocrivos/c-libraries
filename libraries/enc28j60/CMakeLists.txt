file(GLOB ENC28J60_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(enc28j60 ${ENC28J60_SOURCES})

target_include_directories(enc28j60
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(enc28j60 PRIVATE c_std_99)

target_compile_options(enc28j60 PRIVATE -Wall -Wextra -Wpedantic)
