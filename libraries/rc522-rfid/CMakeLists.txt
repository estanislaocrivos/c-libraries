file(GLOB RC522_RFID_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(rc522-rfid ${RC522_RFID_SOURCES})

target_include_directories(rc522-rfid
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_compile_features(rc522-rfid PRIVATE c_std_99)

target_compile_options(rc522-rfid PRIVATE -Wall -Wextra -Wpedantic)
