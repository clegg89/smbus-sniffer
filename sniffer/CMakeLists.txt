cmake_minimum_required (VERSION 3.27)

set(APP_NAME smbus-sniffer)

add_executable(${APP_NAME})

target_compile_options(${APP_NAME} PRIVATE -g3)

add_subdirectory(app)
add_subdirectory(gcc)
add_subdirectory(submodules)
