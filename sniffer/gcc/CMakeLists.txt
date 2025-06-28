target_sources(${APP_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/startup_stm32g070xx.s")

target_link_options(${APP_NAME} PRIVATE
  -T "${CMAKE_CURRENT_LIST_DIR}/stm32g070rbtx_flash.ld"
  -Wl,-Map "$<TARGET_FILE_DIR:${APP_NAME}>/${APP_NAME}.map"
)

# Force re-link if linker script changes
get_target_property(LINK_DEPENDS ${APP_NAME} LINK_DEPENDS)

if(LINK_DEPENDS)
  list(APPEND LINK_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/stm32g070rbtx_flash.ld")
else()
  set(LINK_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/stm32g070rbtx_flash.ld")
endif()

set_target_properties(${APP_NAME} PROPERTIES LINK_DEPENDS "${LINK_DEPENDS}")

