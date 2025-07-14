#pragma once
//! @file

#include <cstdint>
#include <span>

class I2cFakeDevice {
private:
  const uint8_t address_;

public:
  constexpr I2cFakeDevice(uint8_t address) : address_{address} {}

  uint8_t address() const {
    return address_;
  }

  virtual void process(bool isRead, std::span<std::pair<uint8_t, bool>> data) = 0;
};

template <uint8_t ADDR>
class StaticI2cFakeDevice : public I2cFakeDevice {
public:
  constexpr StaticI2cFakeDevice() : I2cFakeDevice{ADDR} {}
};

