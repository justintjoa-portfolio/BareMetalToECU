#include "firmware.h"

#include <iostream>

namespace {

// These constants stand in for linker-generated symbols.
// In real firmware, names like _sidata, _sdata, _edata,
// _sbss, and _ebss come from the linker script.
constexpr uint32_t kDataLoadAddress = 0x08000200;  // Flash source
constexpr uint32_t kDataRunAddress = 0x20000000;   // RAM destination

constexpr uint32_t kBssStartAddress = 0x20000100;
constexpr uint32_t kBssSizeBytes = 4;

constexpr uint32_t kGpioABase = 0x48000000;
constexpr uint32_t kGpioAModer = kGpioABase + 0x00;
constexpr uint32_t kGpioAOdr = kGpioABase + 0x14;

} 

namespace baremetal {

Firmware::Firmware(Uart& uart, MemoryMap& memory_map): uart_(uart), memory_map_(memory_map) {}

void Firmware::CopyDataSection() {
  std::cout << "Copying .data from Flash to RAM\n";

  auto value = memory_map_.NormalizeAddressAndRead32(kDataLoadAddress);
  memory_map_.NormalizeAddressAndWrite32(kDataRunAddress, value);
}

void Firmware::ZeroBssSection() {
  std::cout << "Zeroing .bss in RAM\n";
  memory_map_.NormalizeAddressAndWrite32(kBssStartAddress, 0);
}

void Firmware::ResetHandler() {
  CopyDataSection();
  ZeroBssSection();
}

void Firmware::ResetHandlerAndMain() {
  ResetHandler();
  // After handling reset, run 
  Main();
}

// This represents:
//
// int main() {
//
//   UART_Print("Hello World");
//
// }
//
// Important:
//
// The CPU never calls main directly.
//
// Reset_Handler calls main.
//
// This is one of the major differences between:
//
//   Application Programming:
//
//     main() is the beginning.
//
// and
//
//   Embedded Firmware:
//
//     Power On is the beginning.
//
void Firmware::Main() {

  // In real embedded systems this would likely become:
  //
  // UART0->DR = 'H';
  //
  // or:
  //
  // HAL_UART_Transmit(...)
  //
  // Here we fake UART using std::cout.
  //
  memory_map_.NormalizeAddressAndWrite32(kGpioAModer, 0x400);
  memory_map_.NormalizeAddressAndWrite32(kGpioAOdr, 1u << 5);
  memory_map_.NormalizeAddressAndWrite32(kGpioAOdr, 0);
  uart_.Write(
      "Hello World from mock bare metal Cortex-M");
}

}  // namespace baremetal