cmake_minimum_required(VERSION 3.13)

project(common-c-libs
    LANGUAGES C
    DESCRIPTION "Common portable C libraries module"
)

option(BUILD_SHARED_LIBS "Build as shared library" OFF)
option(BUILD_DEMO "Build demo main executable" OFF)

# Add modules/libraries
add_subdirectory(hamming_codec)

if(BUILD_DEMO)
    add_executable(demo src/main.c)
    target_include_directories(demo PRIVATE inc)
    target_link_libraries(demo PRIVATE hamming_codec)
endif()
