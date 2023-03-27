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

    template <typename T>
    void add_child(GCPtr<T> parent, GCPtr<T> child)
    {
        parent->add_child(child.get());
    }

private:
    GarbageCollector &gc_;
};
