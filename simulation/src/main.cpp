#include "src/cpu.h"
#include "src/firmware.h"
#include "src/memory_map.h"
#include "src/st_link.h"
#include "src/uart.h"

int main() {
  baremetal::MemoryMap memory_map;
  baremetal::Cpu cpu(memory_map);
  baremetal::Uart uart;
  baremetal::Firmware firmware(uart, memory_map);

  baremetal::StLink st_link;
  // Here we're loading the firmware image by loading its bytes
  // into flash. STLink is out of the picture after this, it only is for
  // is for loading firmware onto flash.
  st_link.ProgramFlash(memory_map);

  // On power on.
  cpu.Reset();

  // The CPU will run the instructions at the program counter which is the
  // reset handler in the firmware image.
  firmware.ResetHandlerAndMain();

  return 0;
}