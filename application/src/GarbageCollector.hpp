#ifndef GARBAGE_COLLECTOR_HPP
#define GARBAGE_COLLECTOR_HPP

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>
#include <cstring>
#include <unordered_set>

class GarbageCollector;
class GCObjectBase;

template <typename T>
class GCObject;

template <typename T>
using GCPtr = std::shared_ptr<GCObject<T>>;

class GCObjectBase
{
public:
    virtual ~GCObjectBase() {}
    virtual void update_ptr(void *new_ptr) = 0;
};

template <typename T>
class GCObject : public GCObjectBase
{
public:
    GCObject(T *ptr, GarbageCollector &gc) : ptr_(ptr), gc_(gc) {}
    GCObject(const GCObject &other) = delete;
    GCObject &operator=(const GCObject &other) = delete;
    ~GCObject()
    {
        gc_.add(this);
    }
    T *get() const
    {
        return ptr_;
    }

    void update_ptr(void *new_ptr) override
    {
        ptr_ = static_cast<T *>(new_ptr);
    }

private:
    T *ptr_;
    GarbageCollector &gc_;
};

class GarbageCollector
{
public:
    GarbageCollector(bool debug = false, bool log_stats = false) : debug_(debug), log_stats_(log_stats)
    {
        thread_ = std::thread(&GarbageCollector::run, this);
    }

    ~GarbageCollector()
    {
        stop_ = true;
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

    void add(GCObjectBase *ptr)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (ptr)
        {
            objects_[ptr] = false;
        }
    }

    void add_to_root_set(GCObjectBase *ptr)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        root_set_.insert(ptr);
    }

    void remove_from_root_set(GCObjectBase *ptr)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        root_set_.erase(ptr);
    }

    void collect()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (debug_)
        {
            std::cout << "Starting garbage collection\n";
        }
        auto start_time = std::chrono::steady_clock::now();

        // mark phase
        for (GCObjectBase *ptr : root_set_)
        {
            mark(ptr);
        }

        std::vector<GCObjectBase *> garbage;
        for (auto &kv : objects_)
        {
            if (!kv.second)
            {
                garbage.push_back(kv.first);
            }
            else
            {
                kv.second = false;
            }
        }

        for (auto ptr : garbage)
        {
            delete ptr;
            objects_.erase(ptr);
        }
        if (debug_)
        {
            std::cout << "Garbage collection completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() << " ms\n";
        }

        if (log_stats_)
        {
            std::cout << "Live objects: " << objects_.size() << std::endl;
            std::cout << "Collected objects: " << garbage.size() << std::endl;
        }
    }

private:
    void run()
    {
        while (!stop_)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            collect();
        }
    }

    void mark(GCObjectBase *ptr)
    {
        if (!ptr)
        {
            return;
        }
        auto it = objects_.find(ptr);
        if (it != objects_.end() && !it->second)
        {
            it->second = true;
        }
    }
    std::unordered_set<GCObjectBase *> root_set_;
    std::mutex mutex_;
    std::unordered_map<GCObjectBase *, bool> objects_;
    std::thread thread_;
    bool stop_ = false;
    bool debug_;
    bool log_stats_;
};
#endif // GARBAGE_COLLECTOR_HPP