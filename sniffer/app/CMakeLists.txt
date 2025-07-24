target_include_directories(${APP_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

target_sources(${APP_NAME} PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/main.c"
)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/stm32")
