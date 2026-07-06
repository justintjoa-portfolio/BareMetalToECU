/* Until further notice, this was vibe coded. Note to self.*/


#ifndef GPIO_H
#define GPIO_H

#include <cstdint>

namespace baremetal {

class Gpio {
 public:
  void Write32(uint32_t offset, uint32_t value);

 private:
  uint32_t moder_ = 0;
  uint32_t odr_ = 0;
};

}  // namespace baremetal

#endif