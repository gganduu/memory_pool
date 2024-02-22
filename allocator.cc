#include <malloc.h>
#include <string.h>

#include "../include/allocator.h"
#include "../include/macros.h"


namespace zz{
constexpr size_t kAlignment = 64;
alignas(kAlignment) static uint8_t zero_size_area[1];


Status SystemAllocator::AllocateAligned(uint64_t size, uint8_t** out){
    if (size == 0){
        *out = zero_size_area;
        return Status::OK();
    }
    int res = posix_memalign(reinterpret_cast<void**>(out), kAlignment, static_cast<size_t>(size));
    if (res == ENOMEM){
        return Status::OutOfMemory("malloc of size ", size, " failed");
    } else if (res == EINVAL){
        return Status::Invalid("invalid alignment parameter: ", kAlignment);
    } else{
        return Status::OK();
    }
}

Status SystemAllocator::ReallocateAligned(uint64_t old_size, uint64_t new_size, uint8_t** ptr){
    uint8_t* pre_ptr = *ptr;
    if (pre_ptr == zero_size_area){
        CHECK_EQ(old_size, 0);
        return Status::OK();
    }
    if (new_size == 0){
        DeallocateAligned(pre_ptr, new_size);
        *ptr = zero_size_area;
        return Status::OK();
    }

    uint8_t* out = nullptr;
    RETURN_OK(AllocateAligned(new_size, &out));
    CHECK(out);
    // copy contents and release old memory chunk
    memcpy(out, *ptr, static_cast<size_t>(new_size));
    free(*ptr);
    *ptr = out;
    return Status::OK();
}

void SystemAllocator::DeallocateAligned(uint8_t* ptr, uint64_t size){
    if(ptr == zero_size_area){
        CHECK_EQ(size, 0);
    } else{
        free(ptr);
        ptr == nullptr;
    }
}

void SystemAllocator::ReleaseUnused(){
#ifdef __GLIBC__
    UNUSED(malloc_trim(0));
#endif
}

} // namespace zz