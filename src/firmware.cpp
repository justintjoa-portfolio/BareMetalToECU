#include "src/firmware.h"

#include <iostream>

namespace baremetal {

Firmware::Firmware(Cpu* cpu, Uart* uart)
    : cpu_(cpu), uart_(uart) {}

// Simulate the moment electrical power is applied to a Cortex-M.
//
// Real hardware:
//
//   Power On
//      ↓
//   CPU Reset
//      ↓
//   Read vector table from Flash
//      ↓
//   SP = *(0x00000000)
//      ↓
//   PC = *(0x00000004)
//      ↓
//   Jump to Reset_Handler
//
// The CPU does NOT know:
//
//   main()
//   C++
//   std::cout
//   UART
//
// It only knows:
//
//   Load SP
//   Load PC
//   Execute instructions at PC
//
void Firmware::PowerOn() {
  std::cout << "Power on\n";

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
  cpu_->Reset(
      vector_table_.initial_sp,
      vector_table_.reset_handler_address);

  std::cout << "SP = 0x" << std::hex << cpu_->sp() << "\n";
  std::cout << "PC = 0x" << std::hex << cpu_->pc() << "\n";

  // In real hardware:
  //
  // The CPU would now jump to the address stored in PC.
  //
  // In our simulator we call ResetHandler() directly.
  //
  ResetHandler();
}

// Reset_Handler is startup code.
//
// This is firmware.
//
// This code runs BEFORE main().
//
// Real startup code often:
//
//   Copy .data from Flash -> RAM
//
//   Zero .bss
//
//   Initialize clocks
//
//   Initialize C/C++ runtime
//
//   Call main()
//
// Linux does something similar before main(), but hides it from you.
//
// Bare metal exposes it.
//
void Firmware::ResetHandler() {
  std::cout << "Reset_Handler running\n";

  // Example:
  //
  // int initialized = 5;
  //
  // Flash:
  //
  //   initialized = 5
  //
  // RAM before startup:
  //
  //   garbage
  //
  // Reset_Handler copies:
  //
  // Flash -> RAM
  //
  std::cout
      << "Stage 0: skipping .data copy and .bss zero for now\n";

  std::cout << "Reset_Handler calls main\n";

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
  uart_->Write(
      "Hello World from mock bare metal Cortex-M");
}

}  // namespace baremetal