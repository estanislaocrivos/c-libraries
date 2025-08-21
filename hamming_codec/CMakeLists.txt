add_library(hamming_codec STATIC hamming_codec.c)
target_include_directories(hamming_codec PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
