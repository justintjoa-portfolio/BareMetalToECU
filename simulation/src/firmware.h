#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <cstdint>

#include "src/cpu.h"
#include "src/memory_map.h"
#include "src/uart.h"

namespace baremetal {

class Firmware {
 public:
  Firmware(Uart& uart, MemoryMap& memory_map);

  Firmware(const Firmware&) = delete; 
  Firmware(Firmware&&) = delete; 
  Firmware& operator=(const Firmware&) = delete; 
  Firmware& operator=(Firmware&&) = delete; 

  void ResetHandlerAndMain();

 private:
  void Main();
  void CopyDataSection();
  void ZeroBssSection();
  void ResetHandler();


  Uart& uart_;
  MemoryMap& memory_map_;
};

}  // namespace baremetal

#endif //FIRMWARE_H