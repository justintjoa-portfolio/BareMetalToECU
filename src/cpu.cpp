#include <iostream>

#include "src/cpu.h"

namespace {
  // This should be mapped correctly at the memory map level. 
  constexpr uint32_t kVectorTableAddress = 0x00000000;
};

namespace baremetal {

Cpu::Cpu(MemoryMap& memory_map): memory_map_(memory_map) {}

uint32_t Cpu::QueryVectorTable(uint32_t offset) {
  return memory_map_.NormalizeAddressAndRead32(kVectorTableAddress + offset);
}

void Cpu::Reset() {
  std::cout << "CPU resetting \n";

  // On Cortex-M, the first entry in the vector table is the
  // initial Stack Pointer.
  //
  // Example:
  //
  // Flash
  //
  // 0x00000000 -> 0x20000400   initial SP
  // 0x00000004 -> Reset_Handler
  //
  // Generally the vector table is assumed to be at 0x00000000 but after going through memory decoder (see memory map)
  // it may be at a slightly offset address (generally its 0x00000000 for portability).
  std::cout << "CPU reads vector_table[0] for initial SP\n";

  // The second entry is the Reset_Handler address.
  //
  // The CPU will begin execution here.
  //
  std::cout << "CPU reads vector_table[1] for Reset_Handler PC\n";

  // Simulate:
  //
  // SP = vector_table[0]
  // PC = vector_table[1]
  //
  // This runs after power on and checks for power supply stability, 
  // and release reset allowed for Cortex CPU.
  stack_pointer_ = QueryVectorTable(0);
  program_counter_ = QueryVectorTable(4);

  std::cout << "SP = 0x" << std::hex << stack_pointer_ << "\n";
  std::cout << "PC = 0x" << std::hex << program_counter_ << "\n";
  std::cout << "Currently running instructions at reset handler found in firmware image" << std::endl;
}

}  // namespace baremetal