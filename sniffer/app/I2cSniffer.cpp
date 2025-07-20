//! @file

#include "I2cSniffer.h"
#include <cstdio>

void I2cSniffer::start() {
  address_ = 0;
  position_ = std::begin(dataBuffer_);
}

bool I2cSniffer::isAddressSet() const {
  return address_ != 0;
}

void I2cSniffer::setRW(bool isRead) {
  isRead_ = isRead;
}

void I2cSniffer::push(uint8_t incByte, bool ack) {
  if (position_ + 1 == std::end(dataBuffer_)) {
    return; // buffer full, error
  }
  (*position_++) = {incByte, ack};
}

void I2cSniffer::stop() {
	printf("a:0x%hhx", address_);
  processor_.process(address_, isRead_, {std::begin(dataBuffer_), position_});
}
