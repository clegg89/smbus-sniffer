set(CMAKE_SYSTEM_NAME Generic CACHE INTERNAL "")
set(CMAKE_SYSTEM_PROCESSOR ARM)

find_program(UTIL_SEARCH_CMD "where")
if(NOT UTIL_SEARCH_CMD)
  find_program(UTIL_SEARCH_CMD "which")
  if(NOT UTIL_SEARCH_CMD)
    message(FATAL_ERROR "Could not find a utility search command")
  endif()
endif()

set(TOOLCHAIN_PREFIX arm-none-eabi-)

execute_process(
	COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
	OUTPUT_VARIABLE BINUTILS_PATH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXECUTABLE_SUFFIX_C   .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

set(TOOLCHAIN_HEX_COMMAND ${CMAKE_OBJCOPY} -O ihex)
set(TOOLCHAIN_BIN_COMMAND ${CMAKE_OBJCOPY} -O binary)

# Add default compiler options
add_compile_options(
  -save-temps
  -Wall
  -Werror
  -Wno-psabi
  -ffunction-sections
  -fdata-sections
  -g
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

add_link_options(
  -Wl,--gc-sections,--relax
  -Wl,--print-memory-usage
  $<$<COMPILE_LANGUAGE:C>:-Wl,--start-group>
    $<$<COMPILE_LANGUAGE:C>:-lc>
    $<$<COMPILE_LANGUAGE:C>:-lm>
  $<$<COMPILE_LANGUAGE:C>:-Wl,--end-group>
  $<$<COMPILE_LANGUAGE:C>:-Wl,--start-group>
    $<$<COMPILE_LANGUAGE:C>:-lstdc++>
    $<$<COMPILE_LANGUAGE:C>:-lsupc++>
  $<$<COMPILE_LANGUAGE:C>:-Wl,--end-group>
)
