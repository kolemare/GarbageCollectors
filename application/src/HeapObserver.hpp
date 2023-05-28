// GCObserver.hpp
#ifndef HEAP_OBSERVER_HPP
#define HEAP_OBSERVER_HPP

class HeapObserver
{
public:
    virtual ~HeapObserver() {}
    virtual void onUpdate() = 0;
};

#endif // HEAP_OBSERVER_HPP
