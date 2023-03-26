#include <string>
#include <vector>
#include <iostream>

class MyClass
{
public:
    MyClass()
    {
        std::cout << "MyClass constructor called" << std::endl;
    }
    ~MyClass()
    {
        std::cout << "MyClass destructor called" << std::endl;
    }
};

class TestObject
{
public:
    TestObject(int a, int b) : a(a), b(b) {}

    int add() { return a + b; }

private:
    int a;
    int b;
};
