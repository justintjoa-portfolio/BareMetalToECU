#include "memory_region.h"

namespace baremetal {

MemoryRegion::MemoryRegion(std::string name, uint32_t base_address, size_t size): name_(name), base_address_(base_address), bytes_(size) {}  

uint32_t MemoryRegion::GetOffset(uint32_t address) const {
    return address - base_address_;
}

uint8_t MemoryRegion::Read8(uint32_t address) const {
    return bytes_[GetOffset(address)];
}
void MemoryRegion::Write8(uint32_t address, uint8_t value) {
    bytes_[GetOffset(address)] = value; 
}

uint32_t MemoryRegion::base_address() const {
    return base_address_;
}
size_t MemoryRegion::size() const {
    return bytes_.size();
}

const std::string& MemoryRegion::name() const {
    return name_;
}

} // namespace baremetal