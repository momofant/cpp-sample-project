#include <cassert>


bool test1()
{
  assert(2 < 1);
}

bool test2()
{
  assert(0);
}

int main()
{
  test1();
  test2();
  return 0;
}
