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
    blocks_.push_back({allocated, size, true});

    return allocated;
}

void MemoryManagement::deallocate(void *ptr)
{
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto &block : blocks_)
    {
        if (block.address == ptr)
        {
            block.isAllocated = false;

            // Find and unregister the pointers
            auto iter = std::find_if(pointerRegistry_.begin(), pointerRegistry_.end(),
                                     [&](const std::pair<GCObjectBase *, void *> &pair)
                                     { return pair.second == ptr; });

            if (iter != pointerRegistry_.end())
            {
                pointerRegistry_.erase(iter);
            }
            return;
        }
    }
    throw std::invalid_argument("Pointer not found in blocks");
}

void MemoryManagement::reset()
{
    std::lock_guard<std::mutex> lock(mtx_);
    blocks_.clear();
    freePointer_ = heap_;
}

void MemoryManagement::compact()
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::sort(blocks_.begin(), blocks_.end(), [](const Block &a, const Block &b)
              { return a.address < b.address; });

    char *compactionPointer = heap_;
    for (auto &block : blocks_)
    {
        if (block.isAllocated && block.address != compactionPointer)
        {
            std::memmove(compactionPointer, block.address, block.size);

            for (auto &entry : pointerRegistry_)
            {
                if (entry.second == block.address)
                {
                    entry.second = compactionPointer;
                    entry.first->update_ptr(compactionPointer);
                }
            }

            block.address = compactionPointer;
        }
        if (block.isAllocated)
            compactionPointer += block.size;
    }

    freePointer_ = compactionPointer;

    notifyObservers();

    // remove all unallocated blocks
    blocks_.erase(std::remove_if(blocks_.begin(), blocks_.end(),
                                 [](const Block &block)
                                 { return !block.isAllocated; }),
                  blocks_.end());
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

void MemoryManagement::registerObserver(HeapObserver *observer)
{
    std::lock_guard<std::mutex> lock(mtx_);
    observers_.push_back(observer);
    std::cout << "Observer registered!" << std::endl;
}

void MemoryManagement::unregisterObserver(HeapObserver *observer)
{
    auto it = std::find(observers_.begin(), observers_.end(), observer);
    if (it != observers_.end())
    {
        observers_.erase(it);
    }
}

std::vector<Block> MemoryManagement::getBlocks() const
{
    return blocks_;
}

std::size_t MemoryManagement::getMaxBlockSize() const
{
    std::size_t maxBlockSize = 0;
    for (const auto &block : blocks_)
    {
        if (block.size > maxBlockSize)
        {
            maxBlockSize = block.size;
        }
    }
    return maxBlockSize;
}

void MemoryManagement::notifyObservers()
{
    for (HeapObserver *observer : observers_)
    {
        observer->onUpdate();
    }
}

MemoryManagement::~MemoryManagement()
{
    delete[] heap_;
}