target_sources(${APP_NAME} PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/I2cConexantDevice.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/I2cSniffer.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/stm32")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/utility")
