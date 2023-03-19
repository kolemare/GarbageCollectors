#include "gtest/gtest.h"
#include "GarbageCollector.hpp"

// Test allocating and deallocating an object
TEST(GarbageCollectorTest, BasicAllocationTest)
{
    GarbageCollector<int> gc;
    gc.start();

    int *ptr = gc.allocate<int>();
    *ptr = 5;

    gc.collect_garbage();

    ASSERT_EQ(*ptr, 5);
}

// Test adding and removing roots
TEST(GarbageCollectorTest, RootTest)
{
    GarbageCollector<int> gc;
    gc.start();

    int *ptr = gc.allocate<int>();
    gc.add_root(ptr);

    // ASSERT_EQ(gc.collect_garbage(), 1);

    gc.remove_root(ptr);

    // ASSERT_EQ(gc.collect_garbage(), 0);
}

// Test marking objects
TEST(GarbageCollectorTest, MarkingTest)
{
    GarbageCollector<int> gc;
    gc.start();

    int *ptr1 = gc.allocate<int>();
    int *ptr2 = gc.allocate<int>();

    // ptr1 points to ptr2
    *ptr1 = reinterpret_cast<std::uintptr_t>(ptr2);
    *ptr2 = 5;

    gc.collect_garbage();

    ASSERT_EQ(*ptr2, 5);

    // Now ptr2 points to ptr1
    *ptr2 = reinterpret_cast<std::uintptr_t>(ptr1);

    gc.collect_garbage();

    ASSERT_EQ(*ptr1, reinterpret_cast<std::uintptr_t>(ptr2));
    ASSERT_EQ(*ptr2, reinterpret_cast<std::uintptr_t>(ptr1));
}

// Test deallocating objects
TEST(GarbageCollectorTest, DeallocatingTest)
{
    GarbageCollector<int> gc;
    gc.start();

    int *ptr1 = gc.allocate<int>();
    int *ptr2 = gc.allocate<int>();

    // ptr1 points to ptr2
    *ptr1 = reinterpret_cast<std::uintptr_t>(ptr2);

    gc.collect_garbage();

    // Deallocate ptr2
    gc.allocate<int>();

    gc.collect_garbage();

    ASSERT_EQ(*ptr1, reinterpret_cast<std::uintptr_t>(ptr2));
}

// Test allocation failure
TEST(GarbageCollectorTest, AllocationFailureTest)
{
    GarbageCollector<int> gc;
    gc.start();

    // Allocate a large amount of memory to trigger an allocation failure
    ASSERT_THROW(gc.allocate<int>(1000000000), std::bad_alloc);
}
