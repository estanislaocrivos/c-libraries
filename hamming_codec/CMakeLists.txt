file(GLOB HAMMING_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(hamming_codec ${HAMMING_SOURCES})

target_include_directories(hamming_codec
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(hamming_codec PRIVATE c_std_99)

# Flags opcionales
target_compile_options(hamming_codec PRIVATE -Wall -Wextra -Wpedantic)
