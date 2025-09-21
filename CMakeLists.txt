cmake_minimum_required(VERSION 3.13)

project(common-c-libs
    LANGUAGES C
    DESCRIPTION "Common portable C libraries module"
)

add_subdirectory(hamming-codec)
add_subdirectory(embedded-hal)
add_subdirectory(lcd-16-by-2)
