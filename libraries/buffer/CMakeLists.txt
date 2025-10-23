file(GLOB NEW_LIB_TEMPLATE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(buffer ${NEW_LIB_TEMPLATE_SOURCES})

target_include_directories(buffer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(buffer PRIVATE c_std_99)

target_compile_options(buffer PRIVATE -Wall -Wextra -Wpedantic)
