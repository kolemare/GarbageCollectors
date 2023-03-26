#include <GarbageCollector.hpp>

class GCFactory
{
public:
    GCFactory(GarbageCollector &gc) : gc_(gc) {}

    template <typename T, typename... Args>
    GCPtr<T> create(Args &&...args)
    {
        T *obj = new T(std::forward<Args>(args)...);
        return std::make_shared<GCObject<T>>(obj, gc_);
    }

private:
    GarbageCollector &gc_;
};
