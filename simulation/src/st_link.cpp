#include "st_link.h"

#include <cstdint>
#include <iostream>

namespace {

// In a real STM32, ST-Link receives a linked firmware image
// such as an ELF/BIN/HEX file and writes those bytes into Flash.
//
// In Stage 1, we manually program the smallest useful firmware image:
//
//   Flash @ 0x08000000
//   [0] initial stack pointer
//   [1] Reset_Handler address
//   [later] initial .data bytes
//
// This models what the linker + ST-Link would have produced.
constexpr std::uint32_t kFlashBaseAddress = 0x08000000;

constexpr std::uint32_t kVectorTableAddress = kFlashBaseAddress;
constexpr std::uint32_t kInitialStackPointer = 0x20000400;
constexpr std::uint32_t kResetHandlerAddress = 0x08000100;

constexpr std::uint32_t kDataLoadAddress = 0x08000200;
constexpr std::uint32_t kInitializedGlobalValue = 5;

}  // namespace

namespace baremetal {

void StLink::ProgramFlash(MemoryMap& memory_map) {
  std::cout << "ST-Link: Programming Flash...\n";

  // --------------------------------------------------------------------------
  // Vector table
  // --------------------------------------------------------------------------
  //
  // The vector table is the first thing in the firmware image.
  //
  // Entry 0:
  //   Initial Stack Pointer value. During reset, the CPU loads this into SP.
  //
  // Entry 1:
  //   Reset_Handler address. During reset, the CPU loads this into PC.
  //
  memory_map.NormalizeAddressAndWrite32(kVectorTableAddress + 0, kInitialStackPointer);
  memory_map.NormalizeAddressAndWrite32(kVectorTableAddress + 4, kResetHandlerAddress);

  // --------------------------------------------------------------------------
  // Initial .data contents
  // --------------------------------------------------------------------------
  //
  // Pretend the firmware contains:
  //
  //   int initialized = 5;
  //
  // The initial value is stored in Flash by the linker/ST-Link process.
  // Reset_Handler (part of firmware image) later copies it into RAM before main() runs
  // when reset happens. Please see CopyDataSection in firmware.cpp.
  memory_map.NormalizeAddressAndWrite32(kDataLoadAddress, kInitializedGlobalValue);

  std::cout << "ST-Link: Flash programming complete.\n";
}

}  // namespace baremetal