#ifndef GC_WRAP_OBJECT_HPP
#define GC_WRAP_OBJECT_HPP

#include "GCFactory.hpp"
#include "MemoryManagement.hpp"
#include <type_traits>
#include <vector>
#include <list>
#include <map>

template <typename T>
class GCWrapObject
{
public:
    // Template Specialization for array types
    template <typename U = T>
    GCWrapObject(GCFactory &factory, std::enable_if_t<std::is_fundamental<U>::value, int> size)
        : gc_obj(factory.create_raw<U>((size_t)size))
    {
        gc_obj->gc().add_to_root_set(gc_obj.get());
        MemoryManagement::getInstance().registerPointer(gc_obj.get(), gc_obj->get());
    }

    // Template Specialization for std::vector
    template <typename U = T>
    GCWrapObject(GCFactory &factory, std::enable_if_t<std::is_same_v<U, std::vector<typename U::value_type>>, size_t> size)
        : gc_obj(factory.create<U>(size))
    {
        gc_obj->gc().add_to_root_set(gc_obj.get());
        MemoryManagement::getInstance().registerPointer(gc_obj.get(), gc_obj->get());
    }

    // Template Specialization for std::list
    template <typename U = T>
    GCWrapObject(GCFactory &factory, std::enable_if_t<std::is_same_v<U, std::list<typename U::value_type>>, size_t> size)
        : gc_obj(factory.create<U>(size))
    {
        gc_obj->gc().add_to_root_set(gc_obj.get());
        MemoryManagement::getInstance().registerPointer(gc_obj.get(), gc_obj->get());
    }

    // Template Specialization for std::map
    template <typename U = T>
    GCWrapObject(GCFactory &factory, std::enable_if_t<std::is_same_v<U, std::map<typename U::key_type, typename U::mapped_type>>, size_t> size)
        : gc_obj(factory.create<U>())
    {
        gc_obj->gc().add_to_root_set(gc_obj.get());
        MemoryManagement::getInstance().registerPointer(gc_obj.get(), gc_obj->get());
    }

    // General template for all types of classes
    template <typename... Args>
    GCWrapObject(GCFactory &factory, Args &&...args)
        : gc_obj(factory.create<T>(std::forward<Args>(args)...))
    {
        gc_obj->gc().add_to_root_set(gc_obj.get());
        MemoryManagement::getInstance().registerPointer(gc_obj.get(), gc_obj->get());
    }

    ~GCWrapObject()
    {
        gc_obj->gc().remove_from_root_set(gc_obj.get());
    }

    T *operator->()
    {
        return gc_obj->get();
    }
    T &operator*()
    {
        return *gc_obj->get();
    }

    template <typename ChildT>
    void add_child(GCWrapObject<ChildT> &child)
    {
        GarbageCollector &gc = gc_obj->gc();
        if (is_reachable_from_other_root(gc.get_root_set(), child.get_gc_obj().get()))
        {
            gc.remove_from_root_set(child.get_gc_obj().get());
        }
        gc_obj->add_child(child.get_gc_obj().get());
    }

    template <typename ChildT>
    void remove_child(GCWrapObject<ChildT> &child)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        gc_obj->remove_child(child.get_gc_obj().get());
    }

    GCPtr<T> &get_gc_obj()
    {
        return gc_obj;
    }

    T &get_object()
    {
        return *gc_obj->get();
    }

private:
    GCPtr<T> gc_obj;
    std::vector<GCObjectBase *> processed;

    bool is_reachable_from_other_root(const std::unordered_set<GCObjectBase *> &root_set, GCObjectBase *target)
    {
        for (auto root : root_set)
        {
            if (root == gc_obj.get())
            {
                continue;
            }
            if (is_reachable(root, target))
            {
                processed.clear();
                return true;
            }
        }
        processed.clear();
        return false;
    }

    bool is_reachable(GCObjectBase *current, GCObjectBase *target)
    {
        if (current == target)
        {
            return true;
        }
        if (std::find(processed.begin(), processed.end(), current) != processed.end())
        {
            return false;
        }
        for (auto child : current->get_children())
        {
            processed.push_back(current);
            if (is_reachable(child, target))
            {
                return true;
            }
        }
        return false;
    }
};

#endif // GC_WRAP_OBJECT_HPP