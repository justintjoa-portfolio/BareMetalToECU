#include "memory_map.h"

namespace {

constexpr uint32_t kBootAliasStartingAddr = 0x00000000;
constexpr uint32_t kFlashStartingAddr = 0x08000000;
constexpr uint32_t kRamStartingAddr = 0x20000000;

constexpr uint32_t kBootAliasSize = 1024;
constexpr uint32_t kFlashSize = 1024;
constexpr size_t kRamSize = 1024;

constexpr uint32_t kGpioAStartingAddr = 0x48000000;
constexpr uint32_t kGpioASize = 1024;

uint32_t NormalizeAddress(uint32_t address) {
  if (address >= kBootAliasStartingAddr &&
      address < kBootAliasStartingAddr + kBootAliasSize) {
    return kFlashStartingAddr + address;
  }

  return address;
}

}  // namespace

namespace baremetal {

MemoryMap::MemoryMap()
    : flash_("FLASH", kFlashStartingAddr, kFlashSize),
      ram_("RAM", kRamStartingAddr, kRamSize) {}

uint32_t MemoryMap::NormalizeAddressAndRead32(uint32_t address) const {
  const uint32_t normalized_address = NormalizeAddress(address);

  const MemoryRegion& region =
      (normalized_address < kRamStartingAddr) ? flash_ : ram_;

  return static_cast<uint32_t>(region.Read8(normalized_address + 0)) |
         (static_cast<uint32_t>(region.Read8(normalized_address + 1)) << 8) |
         (static_cast<uint32_t>(region.Read8(normalized_address + 2)) << 16) |
         (static_cast<uint32_t>(region.Read8(normalized_address + 3)) << 24);
}

void MemoryMap::NormalizeAddressAndWrite32(uint32_t address,
                                           uint32_t value) {
  if (address >= kGpioAStartingAddr &&
    address < kGpioAStartingAddr + kGpioASize) {
    gpioa_.Write32(address - kGpioAStartingAddr, value);
    return;
  }                                         
  
  const uint32_t non_gpio_normalized_address = NormalizeAddress(address);

  MemoryRegion& region =
      (non_gpio_normalized_address < kRamStartingAddr) ? flash_ : ram_;

  region.Write8(non_gpio_normalized_address + 0, static_cast<uint8_t>(value & 0xFF));
  region.Write8(non_gpio_normalized_address + 1, static_cast<uint8_t>((value >> 8) & 0xFF));
  region.Write8(non_gpio_normalized_address + 2, static_cast<uint8_t>((value >> 16) & 0xFF));
  region.Write8(non_gpio_normalized_address + 3, static_cast<uint8_t>((value >> 24) & 0xFF));
}

}  // namespace baremetal