#include "src/GarbageCollector.hpp"
#include "src/GCFactory.hpp"
#include "include/models.hpp"
#include "src/GCWrapObject.hpp"
#include <iostream>
#include "src/GCVisualizer.hpp"
#include "src/HeapVisualizer.hpp"
#include "src/MemoryManagement.hpp"

void simulation1();
void simulation2();

int main()
{
    // simulation1();
    simulation2();
    return 0;
}

void simulation1()
{
    GarbageCollector gc(true, true); // Enable debugging and log statistics
    GCFactory factory(gc);

    GCVisualizer gcVisualizer(gc); // Create an instance of GCVisualizer
    HeapVisualizer heapVisualizer; // Create and register HeapVisualizer

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

    parent.add_child(child2);

    parent.add_child(child3);

    parent.add_child(child4);

    child1.add_child(grandchild1);

    child1.add_child(grandchild2);

    child3.add_child(grandchild3);

    child3.add_child(grandchild4);

    grandchild1.add_child(great_grandchild1);

    grandchild1.add_child(great_grandchild2);

    grandchild3.add_child(int_array);

    grandchild3.add_child(int_str_map);

    grandchild3.add_child(int_vector);

    std::cout << "Sleeping after parent was set to be root!" << std::endl;

    parent.remove_child(child3);
    std::cout << "Sleeping after child3 was removed as a child of parent!" << std::endl;

    parent.remove_child(child1);
    std::cout << "Sleeping after child1 was removed as a child of parent!" << std::endl;

    gc.remove_from_root_set(parent.get_gc_obj().get());
    std::cout << "Sleeping after parent was removed from root set!" << std::endl;
}

void simulation2()
{
    GarbageCollector gc(true, true); // Enable debugging and log statistics
    GCFactory factory(gc);

    GCVisualizer gcVisualizer(gc); // Create an instance of GCVisualizer
    HeapVisualizer heapVisualizer; // Create and register HeapVisualizer
    std::string longString = "";
    for (int i = 0; i < 20; ++i)
    {
        longString += "A";
    }

    GCWrapObject<ThirdClass> child_5(factory, 3452.325);                // child_5                      7
    GCWrapObject<AnotherClass> grandgrandchild_6(factory, longString);  // grandgrandchild_6            18
    GCWrapObject<ThirdClass> grandgrandchild_3(factory, 875.568);       // grandgrandchild_3            15
    GCWrapObject<MyClass> root_1(factory, 999);                         // root_1                       1
    GCWrapObject<MyClass> grandchild_2(factory, 675);                   // grandchild_2                 9
    GCWrapObject<TestObject> grandgrandchild_4(factory, 1, 2, 3);       // grandgrandchild_4            16
    GCWrapObject<ThirdClass> child_1(factory, 200.15);                  // child_1                      3
    GCWrapObject<AnotherClass> child_4(factory, longString);            // child_4                      6
    GCWrapObject<ThirdClass> grandchild_4(factory, 5685.657);           // grandchild_4                 11
    GCWrapObject<AnotherClass> grandgrandchild_2(factory, longString);  // grandgrandchild_2            14
    GCWrapObject<MyClass> grandgrandchild_1(factory, 56854);            // grandgrandchild_1            13
    GCWrapObject<AnotherClass> root_2(factory, longString);             // root_2                       2
    GCWrapObject<TestObject> grandgrandchild_8(factory, 14, 752, 5673); // grandgrandchild_8            20
    GCWrapObject<MyClass> child_3(factory, 3456);                       // child_3                      5
    GCWrapObject<TestObject> grandchild_5(factory, 156, 762, 863);      // grandchild_5                 12
    GCWrapObject<AnotherClass> grandchild_3(factory, longString);       // grandchild_3                 10
    GCWrapObject<TestObject> child_2(factory, 1, 2, 3);                 // child_2                      4
    GCWrapObject<ThirdClass> grandgrandchild_7(factory, 734.2685);      // grandgrandchild_7            19
    GCWrapObject<TestObject> grandchild_1(factory, 45, 46, 24);         // grandchild_1                 8
    GCWrapObject<MyClass> grandgrandchild_5(factory, 534);              // grandgrandchild_5            17

    root_1.add_child(child_1);
    root_1.add_child(child_2);
    root_2.add_child(child_3);
    root_2.add_child(child_4);
    root_2.add_child(child_5);
    child_1.add_child(grandchild_1);
    child_2.add_child(grandchild_2);
    child_2.add_child(grandchild_3);
    child_3.add_child(grandchild_4);
    child_4.add_child(grandchild_5);
    grandgrandchild_1.add_child(grandchild_1);
    grandchild_1.add_child(grandgrandchild_2);
    grandgrandchild_2.add_child(grandgrandchild_1);
    grandchild_2.add_child(grandgrandchild_3);
    grandchild_4.add_child(grandgrandchild_4);
    grandchild_4.add_child(grandgrandchild_5);
    grandchild_3.add_child(grandgrandchild_5);
    grandchild_4.add_child(grandgrandchild_6);
    grandchild_5.add_child(grandgrandchild_6);
    grandchild_5.add_child(grandgrandchild_7);
    grandchild_5.add_child(grandgrandchild_8);
    grandgrandchild_8.add_child(grandchild_5);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after child_2 was removed as a child of root_1!" << std::endl;
    root_1.remove_child(child_2);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after child_3 was removed as a child of root_2!" << std::endl;
    root_2.remove_child(child_3);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after grandchild_5 was removed as a child of child_4!" << std::endl;
    child_4.remove_child(grandchild_5);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after root_2 was removed from root set!" << std::endl;
    gc.remove_from_root_set(root_2.get_gc_obj().get());
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after grandgrandchild_2 was removed as a child of grandchild_1!" << std::endl;
    grandchild_1.remove_child(grandgrandchild_2);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after root_1 was removed from root set!" << std::endl;
    gc.remove_from_root_set(root_1.get_gc_obj().get());
    std::this_thread::sleep_for(std::chrono::seconds(3));
}