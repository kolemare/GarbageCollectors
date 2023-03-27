#include "src/GarbageCollector.hpp"
#include "src/GCFactory.hpp"
#include "include/models.hpp"
#include "src/GCWrapObject.hpp"
#include <iostream>

int main()
{
    GarbageCollector gc(true, true); // Enable debugging and log statistics
    GCFactory factory(gc);

    {
        GCWrapObject<MyClass> obj1(factory, 42);
        GCWrapObject<MyClass> obj2(factory, 1337);

        std::cout << "Object 1 value: " << obj1->get_value() << std::endl;
        std::cout << "Object 2 value: " << obj2->get_value() << std::endl;

        // Create an object with a child object
        GCPtr<MyClass> parent = factory.create<MyClass>(999);
        GCPtr<MyClass> child = factory.create<MyClass>(1000);
        factory.add_child(parent, child);

        // Add parent to root set
        gc.add_to_root_set(parent.get());

        std::this_thread::sleep_for(std::chrono::seconds(3));

        gc.remove_from_root_set(parent.get());

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    return 0;
}