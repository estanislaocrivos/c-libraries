cmake_minimum_required(VERSION 3.13)

project(common-c-libs
    LANGUAGES C
    DESCRIPTION "Common portable C libraries module"
)

add_subdirectory(libraries/hamming-codec)
add_subdirectory(libraries/embedded-hal)
add_subdirectory(libraries/hd44780)
add_subdirectory(libraries/crc)
add_subdirectory(libraries/rc522-rfid)
add_subdirectory(libraries/buffer)
add_subdirectory(libraries/ring-buffer)
add_subdirectory(libraries/esp8266ex-wifi)
add_subdirectory(libraries/framing)
add_subdirectory(libraries/logging)
