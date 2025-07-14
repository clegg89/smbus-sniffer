#pragma once
//! @file

#include "I2cSnifferProcessor.h"
#include "I2cFakeDevice.h"

#include <vector>

class I2cSnifferProcessorAggregate : public I2cSnifferProcessor {
private:
  std::vector<I2cFakeDevice*> devices_{};

public:
  constexpr I2cSnifferProcessorAggregate() = default;

  void push(I2cFakeDevice& device) {
    devices_.push_back(&device);
  }

  void process(uint8_t address, bool isRead, std::span<std::pair<uint8_t, bool>> data) override final {
    for (auto & device : devices_) {
      if (device->address() == address) {
        device->process(isRead, data);
      }
    }
  }
};

