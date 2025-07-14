#pragma once
//! @file

#include "I2cSnifferProcessor.h"

#include <cstdint>
#include <array>

class I2cSniffer {
private:
  //! Address of current operation
  uint8_t address_ = 0;

  //! Whether current operation is a read or write
  bool isRead_ = false;

  //! Buffer of data coming in
  //! 1 byte for command/register, 2 bytes for value (typically only 1 given)
  std::array<std::pair<uint8_t, bool>, 3> dataBuffer_{};

  //! Iterator for current position in buffer
  typename decltype(dataBuffer_)::iterator position_ = std::begin(dataBuffer_);

  //! Object that processes incoming commands
  I2cSnifferProcessor& processor_;

public:
  I2cSniffer(I2cSnifferProcessor& processor) : processor_{processor} {}

  void start();

  bool isAddressSet() const;

  void setRW(bool isRead);

  void push(uint8_t incByte, bool ack);

  void stop();
};
