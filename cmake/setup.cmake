## Common helper functions and such for the build system

# Setup toolchain, at the moment only supporting GCC, specifically arm-none-eabi-gcc
include("${CMAKE_CURRENT_LIST_DIR}/toolchain/arm-none-eabi-gcc.cmake")

# Setup MCU
include("${CMAKE_CURRENT_LIST_DIR}/mcu/mcu.cmake")

