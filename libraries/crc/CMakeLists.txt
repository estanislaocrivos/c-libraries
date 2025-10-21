file(GLOB NEW_LIB_TEMPLATE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(crc ${NEW_LIB_TEMPLATE_SOURCES})

target_include_directories(crc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(crc PRIVATE c_std_99)

target_compile_options(crc PRIVATE -Wall -Wextra -Wpedantic)
