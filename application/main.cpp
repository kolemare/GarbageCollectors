#include "src/GarbageCollector.hpp"
#include "src/GCFactory.hpp"
#include "include/models.hpp"
#include "src/GCWrapObject.hpp"
#include <iostream>
#include "src/GCVisualizer.hpp"

int main()
{
    GarbageCollector gc(true, true); // Enable debugging and log statistics
    GCFactory factory(gc);

    GCVisualizer visualizer(gc);      // Create an instance of GCVisualizer
    gc.registerObserver(&visualizer); // Register visualizer as an observer

    GCWrapObject<MyClass> parent(factory, 999);
    GCWrapObject<MyClass> child1(factory, 1000);
    GCWrapObject<AnotherClass> child2(factory, "World");
    GCWrapObject<ThirdClass> child3(factory, 2.71);
    GCWrapObject<AnotherClass> child4(factory, "Universe");

    GCWrapObject<MyClass> grandchild1(factory, 1001);
    GCWrapObject<ThirdClass> grandchild2(factory, 1.61);
    GCWrapObject<AnotherClass> grandchild3(factory, "Milky Way");
    GCWrapObject<ThirdClass> grandchild4(factory, 0.618);

    GCWrapObject<MyClass> great_grandchild1(factory, 1002);
    GCWrapObject<AnotherClass> great_grandchild2(factory, "Solar System");

    // Array
    GCWrapObject<int> int_array(factory, 10);

    // Map
    GCWrapObject<std::map<int, std::string>> int_str_map(factory);

    // Vector
    GCWrapObject<std::vector<int ***>> int_vector(factory);

    parent.add_child(child1);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    parent.add_child(child2);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    parent.add_child(child3);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    parent.add_child(child4);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    child1.add_child(grandchild1);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    child1.add_child(grandchild2);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    child3.add_child(grandchild3);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    child3.add_child(grandchild4);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    grandchild1.add_child(great_grandchild1);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    grandchild1.add_child(great_grandchild2);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    grandchild3.add_child(int_array);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    grandchild3.add_child(int_str_map);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    grandchild3.add_child(int_vector);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after parent was set to be root!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    parent.remove_child(child3);
    std::cout << "Sleeping after child3 was removed as a child of parent!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    parent.remove_child(child1);
    std::cout << "Sleeping after child1 was removed as a child of parent!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    gc.remove_from_root_set(parent.get_gc_obj().get());
    std::cout << "Sleeping after parent was removed from root set!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

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