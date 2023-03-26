#include "src/GarbageCollector.hpp"
#include "src/GCFactory.hpp"
#include "include/models.hpp"
#include "src/GCWrapObject.hpp"
#include <iostream>

int main()
{
    GarbageCollector gc(true, true);
    GCFactory factory(gc);

    {
        GCWrapObject<TestObject> obj1(factory, 1, 2);
        std::cout << "1 + 2 = " << obj1->add() << std::endl;

        GCWrapObject<TestObject> obj2(factory, 3, 4);
        std::cout << "3 + 4 = " << obj2->add() << std::endl;
    }

    {
        GCWrapObject<TestObject> obj3(factory, 5, 6);
        std::cout << "5 + 6 = " << obj3->add() << std::endl;

        GCWrapObject<TestObject> obj4(factory, 7, 8);
        std::cout << "7 + 8 = " << obj4->add() << std::endl;
    }

    GCWrapObject<TestObject> obj5(factory, 17, 18);
    std::this_thread::sleep_for(std::chrono::seconds(15));

    return 0;
}