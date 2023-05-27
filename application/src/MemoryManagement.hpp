#ifndef MEMORYMANAGEMENT_HPP
#define MEMORYMANAGEMENT_HPP

#include <iostream>
#include <vector>
#include <cstddef>
#include <mutex>
#include <unordered_map>
#include <algorithm>

class GCObjectBase; // Forward declaration

const std::size_t HEAP_SIZE = 512 * 1024 * 1024; // 512MB

class MemoryManagement
{
public:
    static MemoryManagement &getInstance();
    MemoryManagement(const MemoryManagement &other) = delete; // Delete copy constructor
    void operator=(const MemoryManagement &other) = delete;   // Delete assignment operator

    void *allocate(std::size_t size);
    void deallocate(void *ptr);
    void reset();
    void compact();
    void registerPointer(GCObjectBase *gcobj, void *ptr);

private:
    MemoryManagement();
    ~MemoryManagement();

    char *heap_;
    char *freePointer_;
    char *heapEnd_;
    std::vector<std::pair<void *, std::size_t>> allocatedBlocks_;
    std::vector<std::pair<void *, std::size_t>> freeBlocks_;
    std::unordered_map<GCObjectBase *, void *> pointerRegistry_;
    std::mutex mtx_;
};

#endif // MEMORYMANAGEMENT_HPP