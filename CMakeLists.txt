cmake_minimum_required (VERSION 3.27)

include(cmake/setup.cmake)

project(sniffer)

enable_language(C CXX ASM)

# TODO make bootloader
# add_subdirectory(bootloader)

add_subdirectory(sniffer)

