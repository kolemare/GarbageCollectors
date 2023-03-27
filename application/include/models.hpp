#include <string>
#include <vector>
#include <iostream>

class MyClass
{
public:
    MyClass(int value) : value_(value) {}
    int get_value() const { return value_; }

private:
    int value_;
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
