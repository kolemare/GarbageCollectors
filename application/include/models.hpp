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

class AnotherClass
{
public:
    AnotherClass(const std::string &name) : name_(name) {}
    std::string get_name() const { return name_; }

private:
    std::string name_;
};

class ThirdClass
{
public:
    ThirdClass(double data) : data_(data) {}
    double get_data() const { return data_; }

private:
    double data_;
};

class TestObject
{
public:
    TestObject(int a, int b, int c) : a(a), b(b), c(c) {}

    int add() { return a + b; }

private:
    int a;
    int b;
    int c;
};
