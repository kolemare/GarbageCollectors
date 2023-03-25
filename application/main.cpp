#include "src/GarbageCollector.hpp"
#include "include/models.hpp"
#include <iostream>

int main()
{
    std::cout << "Here 1" << std::endl;
    GarbageCollector<int> gc;
    std::cout << "Here 2" << std::endl;
    int *ptr = gc.allocate<int>(42);
    std::cout << "Here 3" << std::endl;
    gc.add_root(ptr);
    std::cout << "Here 4" << std::endl;
    // gc.collect_garbage();
    std::cout << "Here 5" << std::endl;
    std::cout.flush();
    return 0;
}