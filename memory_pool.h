#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_

#include <atomic>
#include "status.h"

namespace zz{
namespace internal{

class MemoryPoolStat{
public:
    MemoryPoolStat(): allocated_bytes_(0), max_memory_(0) {}
    int64_t allocated_bytes() const { return this->allocated_bytes_.load(); }
    int64_t max_memory() const { return this->max_memory_.load(); }
    void UpdateAllocatedBytes(int64_t diff){
        int64_t allocated = this->allocated_bytes_.fetch_add(diff) + diff;
        if (allocated > max_memory()){
            this->max_memory_ = allocated;
        }
    }
private:
    std::atomic<int64_t> allocated_bytes_;
    std::atomic<int64_t> max_memory_;
};

} // namespace internal
class MemoryPool{
public:
    virtual ~MemoryPool() = default;
    virtual Status Allocate(int64_t size, uint8_t** out) = 0;
    virtual Status Reallocate(int64_t old_size, int64_t new_size, uint8_t** ptr) = 0;
    virtual void ReleaseUnused() = 0;
    virtual void Free(uint8_t* ptr, int64_t size) = 0;
    virtual int64_t allocated_bytes() const = 0;
    virtual int64_t max_memory() const = 0;
    virtual std::string backend_name() const = 0;
    static std::unique_ptr<MemoryPool> CreateDefault();

protected:
    MemoryPool() = default;
    internal::MemoryPoolStat stat_;
};

} // namespace zz

#endif