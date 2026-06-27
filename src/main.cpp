#include "src/cpu.h"
#include "src/firmware.h"
#include "src/uart.h"

int main() {
  baremetal::Cpu cpu;
  baremetal::Uart uart;
  baremetal::Firmware firmware(&cpu, &uart);

  firmware.PowerOn();

  return 0;
}