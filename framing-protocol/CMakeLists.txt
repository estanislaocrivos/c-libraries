file(GLOB NEW_LIB_TEMPLATE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(new-lib-template ${NEW_LIB_TEMPLATE_SOURCES})

target_include_directories(new-lib-template
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(new-lib-template PRIVATE c_std_99)

target_compile_options(new-lib-template PRIVATE -Wall -Wextra -Wpedantic)
