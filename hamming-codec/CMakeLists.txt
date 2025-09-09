file(GLOB HAMMING_CODEC_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(hamming-codec ${HAMMING_CODEC_SOURCES})

target_include_directories(hamming-codec
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(hamming-codec PRIVATE c_std_99)

target_compile_options(hamming-codec PRIVATE -Wall -Wextra -Wpedantic)
