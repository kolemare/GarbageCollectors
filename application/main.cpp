#include "src/GarbageCollector.hpp"
#include "include/models.hpp"
#include <iostream>

int main()
{

    GarbageCollector<MyClass> gc;
    gc.start();

    MyClass *obj1 = gc.allocate<MyClass>();
    gc.add_root(obj1);

    MyClass *obj2 = gc.allocate<MyClass>();
    gc.add_root(obj2);

    gc.remove_root(obj1);

    MyClass *obj3 = gc.allocate<MyClass>();
    gc.add_root(obj3);

    gc.stop();
    return 0;
}