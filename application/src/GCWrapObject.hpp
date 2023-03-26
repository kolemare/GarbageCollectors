#ifndef GC_WRAP_OBJECT_HPP
#define GC_WRAP_OBJECT_HPP

template <typename T>
class GCWrapObject
{
public:
    template <typename... Args>
    GCWrapObject(GCFactory &factory, Args &&...args) : gc_obj(factory.create<T>(std::forward<Args>(args)...)) {}

    T *operator->() { return gc_obj->get(); }
    T &operator*() { return *gc_obj->get(); }

private:
    GCPtr<T> gc_obj;
};

#endif // GC_WRAP_OBJECT_HPP