#include "src/GarbageCollector.hpp"
#include "src/GCFactory.hpp"
#include "include/models.hpp"
#include "src/GCWrapObject.hpp"
#include <iostream>
#include "src/GCVisualizer.hpp"
#include "src/HeapVisualizer.hpp"
#include "src/MemoryManagement.hpp"
#include <set>

#define maxObjects 17
#define minObjects 15

void simulation1();
void simulation2();
void randomSimulation();

int main()
{
    // simulation2();
    randomSimulation();
    return 0;
}

void randomSimulation()
{
    GarbageCollector gc(true, true); // Enable debugging and log statistics
    GCFactory factory(gc);
    GCVisualizer gcVisualizer(gc); // Create an instance of GCVisualizer
    HeapVisualizer heapVisualizer; // Create and register HeapVisualizer

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distribution(minObjects, maxObjects);
    const int numberOfObjects = distribution(rng);
    std::uniform_int_distribution<int> distributionIndex(0, numberOfObjects - 1);
    GCWrapObject<MyClass> *objects[maxObjects];
    std::vector<std::vector<std::tuple<int, int>>> matrix(numberOfObjects,
                                                          std::vector<std::tuple<int, int>>(numberOfObjects));
    std::vector<std::tuple<int, int>> connectionsToRemove;
    std::set<int> selectedNumbers;

    // Filling object connection matrix with all 0s
    // Matrix of connections between objects represents how connections are established in graph
    for (int i = 0; i < numberOfObjects; ++i)
    {
        for (int j = 0; j < numberOfObjects; ++j)
        {
            matrix[i][j] = std::make_tuple(0, 0);
        }
    }

    for (int i = 0; i < numberOfObjects; ++i)
    {
        objects[i] = new GCWrapObject<MyClass>(factory, distribution(rng));
    }

    for (int i = 0; i < numberOfObjects; ++i)
    {
        int firstRandomConnection = distributionIndex(rng);
        matrix[i][firstRandomConnection] = std::make_tuple(1, 0);
        connectionsToRemove.push_back(std::make_tuple(i, firstRandomConnection));
        if (i == firstRandomConnection)
        {
            int secondRandomConnection = distributionIndex(rng);
            while (secondRandomConnection == i)
            {
                secondRandomConnection = distributionIndex(rng);
            }
            matrix[i][secondRandomConnection] = std::make_tuple(1, 0);
            connectionsToRemove.push_back(std::make_tuple(i, secondRandomConnection));
        }
    }

    for (int i = 0; i < numberOfObjects; ++i)
    {
        int firstRandomConnection = distributionIndex(rng);

        matrix[firstRandomConnection][i] = std::make_tuple(0, 1);
        connectionsToRemove.push_back(std::make_tuple(firstRandomConnection, i));
        if (i == firstRandomConnection)
        {
            int secondRandomConnection = distributionIndex(rng);
            while (secondRandomConnection == i)
            {
                secondRandomConnection = distributionIndex(rng);
            }
            matrix[secondRandomConnection][i] = std::make_tuple(0, 1);
            connectionsToRemove.push_back(std::make_tuple(secondRandomConnection, i));
        }
    }

    for (int i = 5; i < numberOfObjects; ++i)
    {
        for (int j = 0; j < numberOfObjects; ++j)
        {
            if (std::get<0>(matrix[i][j]))
            {
                (*objects[i]).add_child(*objects[j]);
            }
            if (std::get<1>(matrix[i][j]))
            {
                (*objects[j]).add_child(*objects[i]);
            }
        }
    }

    while (connectionsToRemove.size() > 3)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::uniform_int_distribution<size_t> distribution(0, connectionsToRemove.size() - 1);
            size_t randomIndex = distribution(rng);
            std::tuple<int, int> element = connectionsToRemove[randomIndex];
            std::tuple<int, int> element_Matrix = matrix[std::get<0>(element)][std::get<1>(element)];
            if (1 == std::get<0>(element_Matrix))
            {
                (*objects[std::get<0>(element)]).remove_child(*objects[std::get<1>(element)]);
            }
            else if (1 == std::get<1>(element_Matrix))
            {
                (*objects[std::get<1>(element)]).remove_child(*objects[std::get<0>(element)]);
            }
            connectionsToRemove.erase(connectionsToRemove.begin() + randomIndex);
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    gc.empty_root_set();
    std::this_thread::sleep_for(std::chrono::seconds(3));
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

    gc.remove_from_root_set(parent.get_gc_obj().get(), true);
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
    gc.remove_from_root_set(root_2.get_gc_obj().get(), true);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after grandgrandchild_2 was removed as a child of grandchild_1!" << std::endl;
    grandchild_1.remove_child(grandgrandchild_2);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Sleeping after root_1 was removed from root set!" << std::endl;
    gc.remove_from_root_set(root_1.get_gc_obj().get(), true);
    std::this_thread::sleep_for(std::chrono::seconds(3));
}