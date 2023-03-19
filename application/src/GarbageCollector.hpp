#ifndef GARBAGE_COLLECTOR_HPP
#define GARBAGE_COLLECTOR_HPP

#include <atomic>
#include <cstddef>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iostream>

template <typename T>
class GarbageCollector
{
private:
    struct ObjectHeader
    {
        std::atomic<bool> marked;
        size_t size;
        ObjectHeader *next;
    };

    struct MemoryBlock
    {
        void *memory;
        size_t size;
        MemoryBlock *next;
    };

    std::atomic<bool> running;
    std::vector<T *> roots;
    std::mutex roots_mutex;
    MemoryBlock *memory_blocks;
    ObjectHeader *object_list;

    void scan_roots()
    {
        std::lock_guard<std::mutex> lock(roots_mutex);
        for (T *root : roots)
        {
            mark_object(static_cast<ObjectHeader *>(static_cast<void *>(root) - sizeof(ObjectHeader)));
        }
    }

    void scan_objects()
    {
        for (ObjectHeader *object = object_list; object != nullptr; object = object->next)
        {
            if (object->marked)
            {
                object->marked = false;
            }
            else
            {
                std::free(static_cast<void *>(object));
                if (object == object_list)
                {
                    object_list = object->next;
                }
                else
                {
                    for (ObjectHeader *prev = object_list; prev != nullptr; prev = prev->next)
                    {
                        if (prev->next == object)
                        {
                            prev->next = object->next;
                        }
                        std::free(static_cast<void *>(object));
                        object = prev->next;
                    }
                }
                object = object->next;
            }
        }
    }

    void mark_object(ObjectHeader *object)
    {
        if (object && !object->marked)
        {
            object->marked = true;
            for (std::size_t i = 0; i < object->size / sizeof(T); i++)
            {
                mark_object(reinterpret_cast<ObjectHeader *>(reinterpret_cast<T *>(object + 1) + i));
            }
        }
    }

    void sweep_objects(GarbageCollector<T> &gc)
    {
        typename GarbageCollector<T>::ObjectHeader *object = gc.object_list;
        typename GarbageCollector<T>::ObjectHeader *prev = nullptr;

        while (object != nullptr)
        {
            if (object->marked)
            {
                object->marked = false;
                prev = object;
                object = object->next;
            }
            else
            {
                if (prev == nullptr)
                {
                    gc.object_list = object->next;
                }
                else
                {
                    prev->next = object->next;
                }

                typename GarbageCollector<T>::ObjectHeader *next = object->next;
                void *ptr = static_cast<void *>(object);
                std::free(ptr);
                object = next;
            }
        }

        GarbageCollector<T>::MemoryBlock *block = gc.memory_blocks;
        while (block != nullptr && block->next != nullptr)
        {
            if (block->next->memory == nullptr)
            {
                block->next = block->next->next;
            }
            else
            {
                block = block->next;
            }
        }
    }

public:
    GarbageCollector() : running(false), memory_blocks(nullptr), object_list(nullptr)
    {
    }
    ~GarbageCollector()
    {
        stop();
    }

    void start()
    {
        if (running)
        {
            return;
        }
        running = true;
        std::thread collector_thread([this]()
                                     {
        while (running) 
        {
            collect_garbage();
            std::this_thread::sleep_for(std::chrono::seconds(5));
        } });
        collector_thread.detach();
    }

    void stop()
    {
        running = false;
    }

    void *allocate(size_t size)
    {
        void *memory = std::malloc(size);
        if (!memory)
        {
            throw std::bad_alloc();
        }
        MemoryBlock *block = new MemoryBlock{memory, size, memory_blocks};
        memory_blocks = block;
        ObjectHeader *header = static_cast<ObjectHeader *>(memory);
        header->marked = false;
        header->size = size;
        header->next = object_list;
        object_list = header;
        return static_cast<void *>(static_cast<char *>(memory) + sizeof(ObjectHeader));
    }

    template <typename U, typename... Args>
    U *allocate(Args &&...args)
    {
        void *memory = allocate(sizeof(U));
        return new (memory) U(std::forward<Args>(args)...);
    }

    void add_root(T *root)
    {
        std::lock_guard<std::mutex> lock(roots_mutex);
        roots.push_back(root);
    }

    void remove_root(T *root)
    {
        std::lock_guard<std::mutex> lock(roots_mutex);
        auto it = std::find(roots.begin(), roots.end(), root);
        if (it != roots.end())
        {
            roots.erase(it);
        }
    }

    void collect_garbage()
    {
        scan_roots();
        scan_objects();
        sweep_objects(*this);
    }
};
#endif // GARBAGE_COLLECTOR_HPP
