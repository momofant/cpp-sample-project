#include <iostream>
#include <algorithm>

void example11()
{
    int x = -1;
    int y = -1;

    if (x > 0)
        if (y > 0)
            std::cout << "y > 0" << std::endl;
        else
            std::cout << "y <= 0" << std::endl;
    else
        std::cout << "x <= 0" << std::endl;
}

void example12()
{
    int x = 1;

    if (x || (std::cout << "call 1" << std::endl))
    {
        std::cout << "call 2" << std::endl;
    }
}

void example13()
{
    float x = 0.1f;
    if (x)
        std::cout << "this will be printed out" << std::endl;
    if (x - x)
        std::cout << "but this will not" << std::endl;
}

void example14()
{
    char c = 'x';

    if (c == 'x')
        std::cout << "c is x" << std::endl;
    else if (c == 'y')
        std::cout << "c is y" << std::endl;
    else if (c == 'z')
        std::cout << "c is z" << std::endl;
    else
        std::cout << "c is undef" << std::endl;
}

void example17()
{
    {
        int x = 2;

        if (!(--x))
            std::cout << 1;
        else if (!(--x))
            std::cout << 2; // this works
        else if (!(--x))
            std::cout << 3;
        else
            std::cout << -1;
    }

    {

        int x = 2;

        if (!(x--))
            std::cout << 1;
        else if (!(x--))
            std::cout << 2;
        else if (!(x--))
            std::cout << 3; // this works
        else
            std::cout << -1;
    }
}

void example18()
{
    if (auto f = fopen("1.txt", "w"))
    {
        fprintf(f, "sample text");
        fclose(f);
    }
    else
    {
        std::cerr << "can't open file 1.txt for writing" << std::endl;
    }
}

void example19()
{
    int x = -5;
    x = x < 0 ? 0 : x;
    std::cout << x << std::endl;
}


void example20()
{
    int x = 0;
    x < 0 ? 
        std::cout << "first" << std::endl : 
        std::cout << "second" << std::endl;
}

void example21()
{
    int x = 5;
    int y = 
        x < -1 ? -1 : 
        x > 1 ? 1 : x; // clamp function
    std::cout << y << " == " << std::clamp(x, -1, 1) << std::endl;
}

void example22()
{
    int x = 2;

    switch (x)
    {
    case 0: std::cout << "0";
    case 1: std::cout << "1";
    case 2: std::cout << "2";
    case 3: std::cout << "3";
    default: std::cout << "x";
    }
}

void example23()
{
    switch (int x = 2; x + 2)
    {
    case 0: std::cout << "0"; break;
    case 1: std::cout << "1"; break;
    case 2: std::cout << "2"; break;
    case 3: std::cout << "3"; break;
    default: std::cout << "x";
    }
}

void example24()
{
    switch (int x = 3)
    {
    case 0:
        break;
    case 3:
    {
        int k = 5;
        std::cout << k << std::endl;
        break;
    }
    default:
        std::cout << "x";
    }
}

void example25()
{
    goto label;
    std::cout << "won't be shown" << std::endl;
label:
    std::cout << "will be shown" << std::endl;
}


// void example26()
// {
//     goto labelX;
//     int x = 0; // error: jump bypasses variable initialization
// labelX:
//     ++ x;
// }