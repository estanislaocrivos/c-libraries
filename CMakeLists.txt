cmake_minimum_required(VERSION 3.13)

project(common-c-libs
    LANGUAGES C CXX
    DESCRIPTION "Common portable C libraries module"
)

# Allow user to choose library type (default: STATIC)
option(BUILD_SHARED_LIBS "Build as shared library" OFF)

# Optional executable for manual testing (not Ceedling)
option(BUILD_DEMO "Build demo main executable" OFF)

# Source and include directories
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/inc")

# Recursively gather all source files
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.c")

# Remove main.c from the list of sources to avoid conflicts
list(FILTER SOURCES EXCLUDE REGEX "main\\.c$")

# Create library target
add_library(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIR}>
)

# Compiler flags
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)

if(CMAKE_C_COMPILER_ID MATCHES "Clang|AppleClang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Executable for manual testing
if(BUILD_DEMO)
    add_executable(demo "${SOURCE_DIR}/main.c")
    target_include_directories(demo PRIVATE ${INCLUDE_DIR})
    target_link_libraries(demo PRIVATE ${PROJECT_NAME})
endif()
