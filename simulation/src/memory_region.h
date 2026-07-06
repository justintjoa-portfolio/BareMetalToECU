#ifndef MEMORY_REGION_H
#define MEMORY_REGION_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace baremetal {

class MemoryRegion {
 public:

  MemoryRegion(std::string name, uint32_t base_address, size_t size);
  ~MemoryRegion() = default;


  uint8_t Read8(uint32_t address) const;
  void Write8(uint32_t address, uint8_t value);

  uint32_t base_address() const;
  size_t size() const;
  const std::string& name() const;

 private:
  std::string name_;
  uint32_t base_address_;
  std::vector<uint8_t> bytes_;

  uint32_t GetOffset(uint32_t address) const;
};

} // namespace baremetal

#endif // MEMORY_REGION_H