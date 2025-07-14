target_include_directories(${APP_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

target_sources(${APP_NAME} PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx_hal_msp.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx_it.c"
  "${CMAKE_CURRENT_LIST_DIR}/syscalls.c"
  "${CMAKE_CURRENT_LIST_DIR}/sysmem.c"
  "${CMAKE_CURRENT_LIST_DIR}/system_stm32g0xx.c"
)

