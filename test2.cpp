#include <cassert>


void test1()
{
}

void test2()
{
  assert(1);
}

int main()
{
  test1();
  test2();
  return 0;
}
