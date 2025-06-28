cmake_minimum_required (VERSION 3.27)

project(sniffer)

enable_language(C CXX ASM)

include(cmake/setup.cmake)

# TODO make bootloader
# add_subdirectory(bootloader)

add_subdirectory(sniffer)

