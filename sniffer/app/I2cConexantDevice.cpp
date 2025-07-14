//! @file

#include "I2cConexantDevice.h"
#include <cstdio>

void I2cConexantDevice::process(bool isRead, std::span<std::pair<uint8_t, bool>> data) {
  if (isRead || std::size(data) < 2) {
    // Ignore reads, only care about xbox writing to encoder
    return;
  }

  printf("Conexant: 0x%x set to 0x%x\n", std::get<uint8_t>(data[0]), std::get<uint8_t>(data[1]));
}

