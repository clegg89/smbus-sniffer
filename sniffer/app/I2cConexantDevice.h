#pragma once
//! @file

#include "I2cFakeDevice.h"

class I2cConexantDevice : public StaticI2cFakeDevice<0x45> {
private:

public:
  void process(bool isRead, std::span<std::pair<uint8_t, bool>> data) override final;
};

