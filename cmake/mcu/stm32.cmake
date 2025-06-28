# Currently hard-coded for STM32G0x0

add_compile_options(
  -mthumb
  -mcpu=cortex-m0plus
)

add_link_options(
  -mthumb
  -mcpu=cortex-m0plus
  # https://community.st.com/s/question/0D53W00001vno1oSAA/warning-elffile-has-a-load-segment-with-rwx-permissions
  "-Wl,--no-warn-rwx-segment"
)

