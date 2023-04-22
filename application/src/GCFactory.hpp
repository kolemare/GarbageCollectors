#ifndef GCFACRTORY_HPP
#define GCFACRTORY_HPP

#include "GarbageCollector.hpp"

class GCFactory
{
public:
    GCFactory(GarbageCollector &gc) : gc_(gc) {}

    template <typename T, typename... Args>
    GCPtr<T> create(Args &&...args)
    {
        T *obj = new T(std::forward<Args>(args)...);
        std::allocator<GCObject<T>> allocator;
        return std::allocate_shared<GCObject<T>>(allocator, obj, gc_);
    }

    template <typename T>
    GCPtr<T> create_raw(size_t size)
    {
        T *obj = new T[size];
        std::allocator<GCObject<T>> allocator;
        return std::allocate_shared<GCObject<T>>(allocator, obj, gc_);
    }

    template <typename ParentT, typename ChildT>
    void add_child(GCPtr<ParentT> &parent, GCPtr<ChildT> &child)
    {
        parent->add_child(child.get());
    }

    template <typename ParentT, typename ChildT>
    void remove_child(GCPtr<ParentT> parent, GCPtr<ChildT> &child)
    {
        parent->remove_child(child.get());
    }

private:
    GarbageCollector &gc_;
};

#endif // GCFACRTORY_HPP