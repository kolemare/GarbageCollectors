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
        // Create objects
        GCPtr<MyClass> parent = factory.create<MyClass>(999);

        GCPtr<MyClass> child1 = factory.create<MyClass>(1000);
        GCPtr<AnotherClass> child2 = factory.create<AnotherClass>("World");
        GCPtr<ThirdClass> child3 = factory.create<ThirdClass>(2.71);
        GCPtr<AnotherClass> child4 = factory.create<AnotherClass>("Universe");

        GCPtr<MyClass> grandchild1 = factory.create<MyClass>(1001);
        GCPtr<ThirdClass> grandchild2 = factory.create<ThirdClass>(1.61);
        GCPtr<AnotherClass> grandchild3 = factory.create<AnotherClass>("Milky Way");
        GCPtr<ThirdClass> grandchild4 = factory.create<ThirdClass>(0.618);

        GCPtr<MyClass> great_grandchild1 = factory.create<MyClass>(1002);
        GCPtr<AnotherClass> great_grandchild2 = factory.create<AnotherClass>("Solar System");

        // Build tree
        factory.add_child(parent, child1);
        factory.add_child(parent, child2);
        factory.add_child(parent, child3);
        factory.add_child(parent, child4);

        factory.add_child(child1, grandchild1);
        factory.add_child(child1, grandchild2);
        factory.add_child(child3, grandchild3);
        factory.add_child(child3, grandchild4);

        factory.add_child(grandchild1, great_grandchild1);
        factory.add_child(grandchild1, great_grandchild2);

        // Add parent to root set
        gc.add_to_root_set(parent.get());

        std::cout << "Sleeping after parent was set to be root!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::cout << "Sleeping after parent was removed from root!" << std::endl;
        gc.remove_from_root_set(parent.get());

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    return 0;
}

/*
                parent
             /   |   \    \
            /    |    \    \
        child1 child2 child3 child4
        /  \          /   \
        /    \        /     \
      gc1    gc2    gc3    gc4
       |       \
       |        \
      ggc1     ggc2
*/