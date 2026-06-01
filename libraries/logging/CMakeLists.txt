file(GLOB LOG_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(log ${LOG_SOURCES})

target_include_directories(log
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(log
    PUBLIC
        embedded-hal
)

target_compile_features(log PRIVATE c_std_99)

target_compile_options(log PRIVATE -Wall -Wextra -Wpedantic)
