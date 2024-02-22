#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "status.h"

namespace zz{

class SystemAllocator{
public:
    static Status AllocateAligned(uint64_t size, uint8_t** out);
    static Status ReallocateAligned(uint64_t old_size, uint64_t new_size, uint8_t** ptr);
    static void DeallocateAligned(uint8_t* ptr, uint64_t size);
    static void ReleaseUnused();
};
} // namespace zz

#endif