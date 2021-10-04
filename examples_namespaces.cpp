#include <iostream>
#include <string.h>
#include <chrono>

namespace mylib
{
    inline int strlen(char const* s)
    {
        int n = 0;
        while (s[n] != '\0') ++n;
        return n;
    }
}

void example62()
{
    char const s[] = "привет!";
    std::cout << ::strlen(s) << std::endl; // 13
    std::cout << mylib::strlen(s) << std::endl; // 13
}

namespace A {
    inline int x;
}
namespace A
{
    namespace B {
        inline int y;
    }
}
namespace A::B {
    inline int* const py = &y;
}
void example63()
{
    A::x = 3;
    *A::B::py = 5;
    std::cout << A::B::y + A::x << std::endl;
}

void example64()
{
    {
        // название типа данных
        using std::chrono::system_clock; 
        auto t = system_clock::now();
    }
    {
        // название пространства имён
        using namespace std::chrono;
        auto t = system_clock::now();
    }
    {
        // синоним для типа данных
        using the_clock = std::chrono::system_clock;
        auto t = the_clock::now();
    }
}
