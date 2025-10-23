file(GLOB NEW_LIB_TEMPLATE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(framing ${NEW_LIB_TEMPLATE_SOURCES})

target_include_directories(framing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(framing PRIVATE c_std_99)

target_compile_options(framing PRIVATE -Wall -Wextra -Wpedantic)
