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

    GCWrapObject<MyClass> parentl(factory, 999);
    GCWrapObject<ThirdClass> parent(factory, 19.11);

    GCWrapObject<MyClass> child1l(factory, 700);
    GCWrapObject<AnotherClass> child2l(factory, "Mars");
    GCWrapObject<ThirdClass> child3l(factory, 34.67);
    GCWrapObject<AnotherClass> child4l(factory, "Snickers");

    GCWrapObject<MyClass> child1(factory, 1000);
    GCWrapObject<AnotherClass> child2(factory, "World");
    GCWrapObject<ThirdClass> child3(factory, 2.71);
    GCWrapObject<AnotherClass> child4(factory, "Universe");

    GCWrapObject<MyClass> grandchild1l(factory, 1001);
    GCWrapObject<ThirdClass> grandchild2l(factory, 1.61);
    GCWrapObject<AnotherClass> grandchild3l(factory, "Milky Way");
    GCWrapObject<ThirdClass> grandchild4l(factory, 0.618);

    GCWrapObject<MyClass> grandchild5l(factory, 2023);
    GCWrapObject<ThirdClass> grandchild6l(factory, 7.62);
    GCWrapObject<AnotherClass> grandchild7l(factory, "Andromeda");
    GCWrapObject<ThirdClass> grandchild8l(factory, 5.56);

    GCWrapObject<MyClass> grandchild11(factory, 808);
    GCWrapObject<ThirdClass> grandchild12(factory, 4.04);
    GCWrapObject<AnotherClass> grandchild13(factory, "Milky Way");
    GCWrapObject<ThirdClass> grandchild14(factory, 45.457);

    GCWrapObject<MyClass> great_grandchild1l(factory, 1999);
    GCWrapObject<AnotherClass> great_grandchild2l(factory, "Solar System");
    GCWrapObject<MyClass> great_grandchild3l(factory, 1911);
    GCWrapObject<AnotherClass> great_grandchild4l(factory, "Black Hole");

    // Array
    GCWrapObject<int> int_array(factory, 10);

    // Map
    GCWrapObject<std::map<int, std::string>> int_str_map(factory);

    // Vector
    GCWrapObject<std::vector<int>> int_vector(factory);

    parentl.add_child(child1l);

    parentl.add_child(child2l);

    parentl.add_child(child3l);

    parentl.add_child(child4l);

    parent.add_child(child1);

    parent.add_child(child2);

    parent.add_child(child3);

    parent.add_child(child4);

    child2.add_child(int_array);

    child2.add_child(int_str_map);

    child2.add_child(int_vector);

    child1l.add_child(grandchild1l);

    child1l.add_child(grandchild2l);

    child2l.add_child(grandchild3l);

    child3l.add_child(grandchild4l);

    child4l.add_child(grandchild5l);

    child4l.add_child(grandchild6l);

    child4l.add_child(grandchild7l);

    child4l.add_child(grandchild8l);

    grandchild3l.add_child(great_grandchild1l);

    grandchild3l.add_child(great_grandchild2l);

    grandchild3l.add_child(great_grandchild3l);

    grandchild4l.add_child(great_grandchild4l);

    great_grandchild3l.add_child(grandchild11);

    grandchild11.add_child(grandchild12);

    grandchild12.add_child(grandchild13);

    grandchild13.add_child(grandchild14);

    std::cout << "Sleeping after parent was set to be root!" << std::endl;

    parentl.remove_child(child3l);
    std::cout << "Sleeping after child3l was removed as a child of parentl!" << std::endl;

    parent.remove_child(child2);
    std::cout << "Sleeping after child2 was removed as a child of parent!" << std::endl;

    parentl.remove_child(child1l);
    std::cout << "Sleeping after child1l was removed as a child of parentl!" << std::endl;

    parent.remove_child(child3);
    std::cout << "Sleeping after child2 was removed as a child of parent!" << std::endl;

    child2l.add_child(parent);
    std::cout << "Sleeping after parent wass added as a child of parentl!" << std::endl;

    parent.remove_child(child2);
    std::cout << "Sleeping after child2 was removed from parent!" << std::endl;

    parentl.remove_child(child2l);
    std::cout << "Sleeping after child2 was removed from parent!" << std::endl;

    gc.remove_from_root_set(parentl.get_gc_obj().get());
    std::cout << "Sleeping after parentl was removed from root set!" << std::endl;

    return 0;
}