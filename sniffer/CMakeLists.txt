cmake_minimum_required (VERSION 3.27)

set(APP_NAME smbus-sniffer)

add_executable(${APP_NAME})

target_compile_options(${APP_NAME} PRIVATE -g3)

set_target_properties(${APP_NAME}
  PROPERTIES
    C_STANDARD 23
    CXX_STANDARD 23
)

add_subdirectory(app)
add_subdirectory(gcc)
add_subdirectory(submodules)
