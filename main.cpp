#include <iostream>
#include <limits>
#include <cstddef>
#include "anothefile.h"


#define CLAMP(val, minval, maxval) \
    ((val) < (minval) ? (minval) : \
     (val) > (maxval) ? (maxval) : (val))

#if defined (DEBUG)
#   define TRACE(...) printf(__VA_ARGS__);
#else
#   define TRACE(...) ;
#endif

void example1()
{
    std::cout << "long long == signed long long int: " << std::is_same<long long, signed long long int>::value << std::endl;
    std::cout << "char == signed char: " << std::is_same<char, signed char>::value << std::endl;
    std::cout << "char == unsigned char: " << std::is_same<char, unsigned char>::value << std::endl;
}

void example2()
{
    int x;
    std::cin >> x;
    std::cout << x << std::endl;
}

void example3()
{
    int a = 1, b = a;
    std::cout << (a == b) << std::endl;
}

void example4()
{
    unsigned int x = 0;
    std::cout << x << " - 1 = " << x - 1 << std::endl; // 4294967295
}

void example5()
{
    int x = 0x7FFFFFFF; 
    unsigned int y = 0x8FFFFFFF; 
    std::cout << x << " < " << y << ": " << (x < y) << std::endl; // x to unsigned is ok
}

void example6()
{
    int x = -2; 
    unsigned int y = 5; 
    std::cout << x << " < " << y << ": " << (x < y) << std::endl; // x to unsigned failed
}

void example7()
{
    TRACE("this message will be shown in DEBUG version only");
}

int main()
{
    std::cout << getmessage() << std::endl;
    example1();
    example2();
    example3();
    example4();
    example5();
    example6();
    example7();
    return 0;
}
