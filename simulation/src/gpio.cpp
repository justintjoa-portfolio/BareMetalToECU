#include "gpio.h"

#include <iostream>

namespace baremetal {

namespace {
constexpr uint32_t kModerOffset = 0x00;
constexpr uint32_t kOdrOffset = 0x14;
constexpr uint32_t kPin5Mask = 1u << 5;
}  // namespace

void Gpio::Write32(uint32_t offset, uint32_t value) {
  if (offset == kModerOffset) {
    moder_ = value;
    std::cout << "GPIOA MODER = 0x" << std::hex << moder_ << "\n";
    return;
  }

  if (offset == kOdrOffset) {
    odr_ = value;
    std::cout << "GPIOA ODR = 0x" << std::hex << odr_ << "\n";

    if ((odr_ & kPin5Mask) != 0) {
      std::cout << "GPIOA pin 5 = HIGH\n";
    } else {
      std::cout << "GPIOA pin 5 = LOW\n";
    }
    return;
  }

  std::cout << "GPIOA unknown register offset 0x" << std::hex << offset << "\n";
}

}  // namespace baremetal