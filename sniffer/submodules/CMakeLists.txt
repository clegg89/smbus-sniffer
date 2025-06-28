target_compile_definitions(${APP_NAME} PRIVATE
  USE_HAL_DRIVER
  STM32G070xx
)

target_include_directories(${APP_NAME} PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/cmsis-core/Include"
  "${CMAKE_CURRENT_LIST_DIR}/cmsis-device-g0/Include"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Inc"
)

target_sources(${APP_NAME} PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_i2c.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_i2c.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_i2c_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_rcc.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_rcc_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_ll_rcc.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_flash.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_flash_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_gpio.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_dma.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_dma_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_pwr.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_pwr_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_cortex.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal.c"
  "${CMAKE_CURRENT_LIST_DIR}/stm32g0xx-hal-driver/Src/stm32g0xx_hal_exti.c"
)

