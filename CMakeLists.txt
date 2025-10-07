cmake_minimum_required(VERSION 3.13)

project(common-c-libs
    LANGUAGES C
    DESCRIPTION "Common portable C libraries module"
)

add_subdirectory(libraries/hamming-codec)
add_subdirectory(libraries/embedded-hal)
add_subdirectory(libraries/lcd-16-by-2)
