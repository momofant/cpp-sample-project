#include <iostream>

void example57()
{
    enum A {
        a1, // = 0
        a2, // = 1
        a3 // = 2
    };

    enum B : uint8_t {
        b1, // = 0
        b2, // = 1
        b3, // = 2
        b100 = 100, // = 100
        b101, // = 101
        b102 // = 102
    };

    std::cout << sizeof(a1) << std::endl;
    std::cout << sizeof(b1) << std::endl;
}

void example58()
{
    enum class A {
        a1 = -1,
    };

    enum class B : uint {
        b1 = ~0u,
        b2 = 0
    };

    std::cout << int(A::a1) << std::endl;
    std::cout << uint(B::b1) << std::endl;
}

enum class Status : int
{
    GeneralError = -1000,
    IvalidArgument,
    OutOfRange,
    Ok = 0,
    Pending
};


inline bool failed(Status status)
{
    return int(status) < 0;
}


Status dosomething(int val)
{
    if (val < 0)
        return Status::IvalidArgument;
    return Status::Ok;
}

void example60()
{
    auto status = dosomething(-4);
    if (failed(status))
        std::cerr << "dosomething failed with code " << int(status) << std::endl;
    else
        std::cout << "dosomething succeeded" << std::endl;
}
