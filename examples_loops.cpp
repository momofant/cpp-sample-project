#include <iostream>

void example27()
{
    int i = 5;
    while (i--)
        std::cout << 'x'; // xxxxx

    i = 5;
    while (--i)
        std::cout << 'o'; // oooo

    i = 5;
    while (true)
    {
        -- i;
        if (i == 2)
            break;

        std::cout << 'w'; // ww
    }
}

void example28()
{
    int i = 4;
    do
        std::cout << i << ' ';
    while (--i);
}

void example29()
{
    do
    {
        bool something_is_wrong = false;
        if (something_is_wrong) break;

        bool something_else_is_wrong = true;
        if (something_else_is_wrong) break;
    }
    while (false);
}

void example30()
{
    // if (expression1 && expression2) {...}
    // if (expression1 || expression2) {...}
    // for (init-statement; condition; expression) statement;
    // for (init-statement; condition; expression) { statement }
    // for (init-statement; condition) statement;

    for (int i = 0; i < 5; ++ i)
    {
        std::cout << i << ' ';
    }
    // 0 1 2 3 4 
    std::cout << std::endl;

    for (int i = 0, j = 5; i < j; i+=2, ++j)
    {
        std::cout << i << ',' << j << "    ";
    }
    // 0,5    2,6    4,7    6,8    8,9  
    std::cout << std::endl;
}
