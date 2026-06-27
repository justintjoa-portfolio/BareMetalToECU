#ifndef UART_H
#define UART_H

#include <string_view>

namespace baremetal {

class Uart {
 public:
  void Write(std::string_view message);
};

}  // namespace baremetal

#endif // UART_H