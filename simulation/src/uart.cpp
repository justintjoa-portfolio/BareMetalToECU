#include "src/uart.h"

#include <iostream>

namespace baremetal {

void Uart::Write(std::string_view message) {
  std::cout << "[UART] " << message << "\n";
}

}  // namespace baremetal