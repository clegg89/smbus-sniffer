#pragma once
//! @file

#include <cstdint>
#include <span>

class I2cSnifferProcessor {
public:
  virtual void process(uint8_t address, bool isRead, std::span<std::pair<uint8_t, bool>> data) = 0;
};

