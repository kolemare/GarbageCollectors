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
#include <GCObserver.hpp>

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
    virtual void add_child(GCObjectBase *child) = 0;
    virtual void remove_child(GCObjectBase *child) = 0;
    virtual const std::vector<GCObjectBase *> &get_children() const = 0;
    virtual void destroy() = 0;
    virtual GarbageCollector &gc() const = 0; // Add this line
};

template <typename T>
class GCObject : public GCObjectBase
{
public:
    GCObject(T *ptr, GarbageCollector &gc) : ptr_(ptr), gc_(gc)
    {
        gc_.add(this);
    }
    GCObject(const GCObject &other) : ptr_(other.ptr_), gc_(other.gc_)
    {
        gc_.add(this);
    }

    GarbageCollector &gc() const override
    {
        return gc_;
    }

    GCObject &operator=(const GCObject &other) = delete;
    ~GCObject()
    {
        gc_.remove(this);
    }
    T *get() const
    {
        return ptr_;
    }

    void destroy() override
    {
        delete ptr_;
    }

    void update_ptr(void *new_ptr) override
    {
        ptr_ = static_cast<T *>(new_ptr);
    }

    void add_child(GCObjectBase *child) override
    {
        children_.push_back(child);
        gc_.notifyObservers();
    }

    void remove_child(GCObjectBase *child) override
    {
        auto it = std::find(children_.begin(), children_.end(), child);
        if (it != children_.end())
        {
            children_.erase(it);
        }
        gc_.notifyObservers();
    }

    const std::vector<GCObjectBase *> &get_children() const override
    {
        return children_;
    }

private:
    T *ptr_;
    GarbageCollector &gc_;
    std::vector<GCObjectBase *> children_;
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

    void registerObserver(GCObserver *observer)
    {
        observers_.push_back(observer);
    }

    void unregisterObserver(GCObserver *observer)
    {
        auto it = std::find(observers_.begin(), observers_.end(), observer);
        if (it != observers_.end())
        {
            observers_.erase(it);
        }
    }

    void notifyObservers()
    {
        for (GCObserver *observer : observers_)
        {
            observer->onUpdate();
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

    void remove(GCObjectBase *ptr)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = objects_.find(ptr);
        if (it != objects_.end())
        {
            objects_.erase(it);
        }
    }

    void add_to_root_set(GCObjectBase *ptr)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        root_set_.insert(ptr);
        notifyObservers();
    }

    void remove_from_root_set(GCObjectBase *ptr)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = root_set_.find(ptr);
        if (it != root_set_.end())
        {
            root_set_.erase(it);
        }
        notifyObservers();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    const std::unordered_set<GCObjectBase *> &get_root_set() const
    {
        return root_set_;
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

        for (auto &ptr : garbage)
        {
            ptr->destroy();
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
            for (GCObjectBase *child : ptr->get_children())
            {
                mark(child);
            }
        }
    }

    std::unordered_set<GCObjectBase *> root_set_;
    std::mutex mutex_;
    std::unordered_map<GCObjectBase *, bool> objects_;
    std::thread thread_;
    bool stop_ = false;
    bool debug_;
    bool log_stats_;
    std::vector<GCObserver *> observers_;
};
#endif // GARBAGE_COLLECTOR_HPP