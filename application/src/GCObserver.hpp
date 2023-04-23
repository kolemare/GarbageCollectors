// GCObserver.hpp
#ifndef GC_OBSERVER_HPP
#define GC_OBSERVER_HPP

class GCObserver
{
public:
    virtual ~GCObserver() {}
    virtual void onUpdate() = 0;
};

#endif // GC_OBSERVER_HPP
