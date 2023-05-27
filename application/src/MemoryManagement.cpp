#include "MemoryManagement.hpp"
#include "GarbageCollector.hpp" // Include this after MemoryManagement.hpp

MemoryManagement &MemoryManagement::getInstance()
{
    static MemoryManagement instance;
    return instance;
}

void *MemoryManagement::allocate(std::size_t size)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (freePointer_ + size > heapEnd_)
    {
        throw std::bad_alloc();
    }

    void *allocated = freePointer_;
    freePointer_ += size;
    allocatedBlocks_.push_back({allocated, size});

    return allocated;
}

void MemoryManagement::deallocate(void *ptr)
{
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto it = allocatedBlocks_.begin(); it != allocatedBlocks_.end();)
    {
        if (it->first == ptr)
        {
            freeBlocks_.push_back(*it);

            // Find and unregister the pointers
            auto iter = std::find_if(pointerRegistry_.begin(), pointerRegistry_.end(),
                                     [&](const std::pair<GCObjectBase *, void *> &pair)
                                     { return pair.second == ptr; });

            if (iter != pointerRegistry_.end())
            {
                pointerRegistry_.erase(iter);
            }

            it = allocatedBlocks_.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
    throw std::invalid_argument("Pointer not found in allocated blocks");
}

void MemoryManagement::reset()
{
    std::lock_guard<std::mutex> lock(mtx_);
    allocatedBlocks_.clear();
    freeBlocks_.clear();
    freePointer_ = heap_;
}

void MemoryManagement::compact()
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::sort(allocatedBlocks_.begin(), allocatedBlocks_.end(), [](const auto &a, const auto &b)
              { return a.first < b.first; });

    char *compactionPointer = heap_;
    for (auto &block : allocatedBlocks_)
    {
        if (block.first != compactionPointer)
        {
            std::memmove(compactionPointer, block.first, block.second);

            for (auto &entry : pointerRegistry_)
            {
                if (entry.second == block.first)
                {
                    entry.second = compactionPointer;
                    entry.first->update_ptr(compactionPointer);
                }
            }

            block.first = compactionPointer;
        }
        compactionPointer += block.second;
    }
    freePointer_ = compactionPointer;
    freeBlocks_.clear();
}

void MemoryManagement::registerPointer(GCObjectBase *gcobj, void *ptr)
{
    std::lock_guard<std::mutex> lock(mtx_);
    pointerRegistry_[gcobj] = ptr;
}

MemoryManagement::MemoryManagement()
    : heap_(new char[HEAP_SIZE]),
      freePointer_(heap_),
      heapEnd_(heap_ + HEAP_SIZE) {}

MemoryManagement::~MemoryManagement()
{
    delete[] heap_;
}