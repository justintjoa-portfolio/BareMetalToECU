#include "src/cpu.h"

namespace baremetal {

void Cpu::Reset(std::uint32_t initial_sp, std::uint32_t reset_pc) {
  sp_ = initial_sp;
  pc_ = reset_pc;
}

std::uint32_t Cpu::pc() const {
  return pc_;
}

std::uint32_t Cpu::sp() const {
  return sp_;
}

}  // namespace baremetal