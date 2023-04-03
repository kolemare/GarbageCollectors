#include <GarbageCollector.hpp>

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

    template <typename ParentT, typename ChildT>
    void add_child(GCPtr<ParentT> parent, GCPtr<ChildT> child)
    {
        parent->add_child(child.get());
    }

private:
    GarbageCollector &gc_;
};
