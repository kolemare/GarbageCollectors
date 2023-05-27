#ifndef MEMORYMANAGEMENT_HPP
#define MEMORYMANAGEMENT_HPP

#include <iostream>
#include <vector>
#include <cstddef>

const std::size_t HEAP_SIZE = 512 * 1024 * 1024; // 512MB

class MemoryManagement
{
public:
    MemoryManagement()
    {
        heap_ = new char[HEAP_SIZE];
        heapEnd_ = heap_ + HEAP_SIZE;
        freePointer_ = heap_;
    }

    ~MemoryManagement()
    {
        delete[] heap_;
    }

    void *allocate(std::size_t size)
    {
        if (freePointer_ + size > heapEnd_)
        {
            throw std::bad_alloc();
        }

        void *allocated = freePointer_;
        freePointer_ += size;
        allocatedBlocks_.push_back({allocated, size});

        return allocated;
    }

    void deallocate(void *ptr)
    {
        for (auto it = allocatedBlocks_.begin(); it != allocatedBlocks_.end(); ++it)
        {
            if (it->first == ptr)
            {
                freeBlocks_.push_back(*it);
                allocatedBlocks_.erase(it);
                return;
            }
        }
        throw std::invalid_argument("Pointer not found in allocated blocks");
    }

    void reset()
    {
        allocatedBlocks_.clear();
        freeBlocks_.clear();
        freePointer_ = heap_;
    }

    // Mark and Compact Algorithm will be implemented in this function
    void compact()
    {
        // TODO
    }

private:
    char *heap_;
    char *heapEnd_;
    char *freePointer_;

    std::vector<std::pair<void *, std::size_t>> allocatedBlocks_;
    std::vector<std::pair<void *, std::size_t>> freeBlocks_;
};

#endif // MEMORYMANAGEMENT_HPP