#include <iostream>
#include "anothefile.h"

std::string getmessage()
{
	return "hello world";
}

void somefunc()
{
	std::cout << &globconst << std::endl;
}
