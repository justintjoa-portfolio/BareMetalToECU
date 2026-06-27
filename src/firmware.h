#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <cstdint>

#include "src/cpu.h"
#include "src/uart.h"

namespace baremetal {

struct VectorTable {
  std::uint32_t initial_sp = 0x20000400;  // Default initial SP value for Cortex-M
  std::uint32_t reset_handler_address = 0x00000100; // Default Reset_Handler address for Cortex-M
};

class Firmware {
 public:
  Firmware(Cpu* cpu, Uart* uart);

  Firmware(const Firmware&) = delete; 
  Firmware(Firmware&&) = delete; 
  Firmware& operator=(const Firmware&) = delete; 
  Firmware& operator=(Firmware&&) = delete; 


  void PowerOn();
  void ResetHandler();
  void Main();

 private:
  Cpu* cpu_;
  Uart* uart_;

  VectorTable vector_table_;
};

}  // namespace baremetal

#endif //FIRMWARE_H