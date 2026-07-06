#ifndef CPU_H
#define CPU_H

#include "memory_map.h"
#include <cstdint>

namespace baremetal {

class Cpu {
 public:
  Cpu(MemoryMap& memory_map);

  ~Cpu() = default;

  Cpu(const Cpu&) = delete;

  Cpu& operator=(const Cpu&) = delete;

  Cpu(Cpu&&) = delete;

  Cpu& operator=(Cpu&&) = delete;

  uint32_t QueryVectorTable(uint32_t offset);

  void Reset();

 private:
  MemoryMap& memory_map_;

  // CPU register that holds the address of the 
  // instruction the CPU is about to execute, or is 
  // currently executing depending on architecture detail.
  std::uint32_t program_counter_ = 0;

  // CPU register that points to the current top of the stack in RAM.
  // The CPU uses the stack to store return addresses, local variables, and saved
  // registers during function calls and interrupts.
  std::uint32_t stack_pointer_ = 0;
};

}  // namespace baremetal

#endif // CPU_H