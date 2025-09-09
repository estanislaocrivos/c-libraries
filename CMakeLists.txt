cmake_minimum_required(VERSION 3.13)

project(common-c-libs
    LANGUAGES C
    DESCRIPTION "Common portable C libraries module"
)

# Add modules/libraries
add_subdirectory(hamming-codec hamming_codec.c)
add_subdirectory(spi-interface spi.c)
