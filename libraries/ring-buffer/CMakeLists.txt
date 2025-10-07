file(GLOB RING_BUFFER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(ring-buffer ${RING_BUFFER_SOURCES})

target_include_directories(ring-buffer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(ring-buffer PRIVATE c_std_99)

target_compile_options(ring-buffer PRIVATE -Wall -Wextra -Wpedantic)
