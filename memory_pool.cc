#include <vector>
#include <limits>
#include <algorithm>

#include "../include/memory_pool.h"
#include "../include/macros.h"
#include "../include/allocator.h"
#include "../include/utils.h"

namespace zz{
constexpr char kDefaultBackendEnv[] = "DEFAULT_MEMORY_POOL";
enum class MemoryPoolBackend: uint8_t { System };
struct SupportedBackend{
    std::string name;
    MemoryPoolBackend backend;
};

std::vector<SupportedBackend> SupportedBackends(){
    std::vector<SupportedBackend> backends{{"System", MemoryPoolBackend::System}};
    return backends;
}

std::optional<MemoryPoolBackend> UserSelectedBackend(){
    auto user_selected_backend = []()->MemoryPoolBackend{
        auto name = GetEnv(kDefaultBackendEnv);
        if (!name.has_value()){
            return {};
        } else{
            std::string str_name = name.value();
            auto found = std::find_if(SupportedBackends().begin(), SupportedBackends().end(), [&](const SupportedBackend& backend){
                return (str_name == backend.name);
            });
            if (!(found==SupportedBackends().end())){
                found->backend;
            }
        }
            
    }();
    return user_selected_backend;
}

template<typename Allocator>
class BaseMemoryPoolImpl : public MemoryPool{
public:
    Status Allocate(int64_t size, uint8_t** out) override {
        if (size < 0){
            return Status::Invalid("negative malloc size");
        }
        if (static_cast<size_t>(size) >= std::numeric_limits<size_t>::max()){
            return Status::CapacityError("malloc size overflows size_t");
        }
        RETURN_OK(Allocator::AllocateAligned(size, out));
        stat_.UpdateAllocatedBytes(size);
        return Status::OK();
    }

    Status Reallocate(int64_t old_size, int64_t new_size, uint8_t** ptr) override {
        if (new_size < 0){
            return Status::Invalid("negative malloc size");
        }
        if (static_cast<size_t>(new_size) >= std::numeric_limits<size_t>::max()){
            return Status::CapacityError("malloc size overflows size_t");
        }
        RETURN_OK(Allocator::ReallocateAligned(old_size, new_size, ptr));
        stat_.UpdateAllocatedBytes(new_size);
        return Status::OK();
    }

    void ReleaseUnused() override { Allocator::ReleaseUnused(); }

    void Free(uint8_t* ptr, int64_t size) override {
        Allocator::DeallocateAligned(ptr, size);
        stat_.UpdateAllocatedBytes(-size);
    }

    int64_t allocated_bytes() const override { return stat_.allocated_bytes(); }

    int64_t max_memory() const override { return stat_.max_memory(); }
};

class SystemMemoryPool : public BaseMemoryPoolImpl<SystemAllocator>{
    std::string backend_name() const override {
        return "System";
    }
};

MemoryPoolBackend DefaultBackend(){
    auto backend = UserSelectedBackend();
    if (backend.has_value()){
        return backend.value();
    } else {
        return MemoryPoolBackend::System;
    }
}

std::unique_ptr<MemoryPool> MemoryPool::CreateDefault(){
    auto backend = DefaultBackend();
    switch (backend)
    {
    case MemoryPoolBackend::System:
        return std::unique_ptr<MemoryPool>(new SystemMemoryPool());
    /* FIXME: support jemalloc in the future. */
    default:
      return nullptr;
    }
}

} // namespace zz