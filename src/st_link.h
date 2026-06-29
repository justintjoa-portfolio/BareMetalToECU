#ifndef ST_LINK_H
#define ST_LINK_H

#include "memory_map.h"

namespace baremetal {

struct StLink {
    StLink() = default; 
    ~StLink() = default;

    StLink(const StLink&) = delete; 
    StLink(StLink&&) = delete; 
    StLink& operator=(const StLink&) = delete; 
    StLink& operator=(StLink&&) = delete; 

    // Programs the firmware image into the target's Flash memory.
    //
    // In a real embedded system, ST-Link receives a firmware image
    // (ELF/BIN/HEX) from the host computer and writes it into the
    // STM32's Flash over the SWD interface.
    //
    // In Stage 1, we simulate this by writing a few bytes directly
    // into Flash.
    void ProgramFlash(MemoryMap& memory_map);
};



} // namespace baremetal


#endif // ST_LINK_H