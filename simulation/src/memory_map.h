#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <cstdint>
#include <memory>

#include "gpio.h"
#include "memory_region.h"

namespace baremetal {

class MemoryMap {
 public:
  MemoryMap();

  // May need to normalize address if implementation chose to offset
  // the addresses of vector table entries. If vector table entry it may 
  // need a form of mapping.
  uint32_t NormalizeAddressAndRead32(uint32_t address) const;
  void NormalizeAddressAndWrite32(uint32_t address, uint32_t value);

 private:
  MemoryRegion flash_;
  MemoryRegion ram_;
  Gpio gpioa_;
}; 

}  // namespace baremetal
#endif // MEMORY_MAP_H