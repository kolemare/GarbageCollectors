#include <mutex>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

class GCInvokeDisplaySingleton
{
public:
    static GCInvokeDisplaySingleton &getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (instance_ == nullptr)
        {
            std::string command = "py GCDisplay.py &";
            system(command.c_str());
            instance_ = new GCInvokeDisplaySingleton();
        }

        return *instance_;
    }

    // Deleted copy constructor and assignment operator to ensure only one instance
    GCInvokeDisplaySingleton(const GCInvokeDisplaySingleton &) = delete;
    GCInvokeDisplaySingleton &operator=(const GCInvokeDisplaySingleton &) = delete;

    // Destructor to perform cleanup actions
    ~GCInvokeDisplaySingleton()
    {
        std::string killCommand = "pkill -f GCDisplay.py";
        system(killCommand.c_str());
    }

private:
    // Private constructor to prevent instantiation
    GCInvokeDisplaySingleton() {}

    // Declare the mutex as static to ensure it is shared across all instances
    static std::mutex mutex_;

    // Private instance variable
    static GCInvokeDisplaySingleton *instance_;
};

// Define the static mutex and instance variables outside the class definition
std::mutex GCInvokeDisplaySingleton::mutex_;
GCInvokeDisplaySingleton *GCInvokeDisplaySingleton::instance_ = nullptr;
