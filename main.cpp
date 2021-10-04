#include <iostream>
#include "examples.h"


int main()
{
    for (auto e : AddExample::examples)
    {
        try
        {
            std::cout << "\033[0;34;48m------ " << e.first << " ------\033[0m" << std::endl;
            e.second();
            std::cout << "\033[0;92;49m------ " << e.first << " ------\033[0m" << std::endl;
        }
        catch (...)
        {
            std::cout << "\033[0;90;41m-- " << e.first << " failed ---\033[0m" << std::endl;
        }
    }

    int x;;
   
    return 0;
}
