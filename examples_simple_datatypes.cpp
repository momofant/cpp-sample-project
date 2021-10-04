#include <iostream>
#include <assert.h>
#include "examples.h"


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

void example8()
{
  int x = 100, y = 30;
  signed char z = static_cast<signed char>(x) + static_cast<signed char>(y);
  std::cout << x << " + " << y << " = " << int(z) << std::endl;
}

void example10()
{
  int x = 0;

  {
    int x = 5;
  }

  std::cout << x << std::endl;
}

void example15()
{
  int x = 1;
  int y = ++++++x;
  std::cout << "++++++1 = " << y << std::endl;
}

void example16()
{
  int x = 1;
  int y = 2;
  int z = x++ + y;
  std::cout << "1 ++ + 2 = " << z << std::endl;
  std::cout << "x = " << x << "; y = " << y << std::endl;
}


void example41()
{
  // types promotion
  short a = std::numeric_limits<short>::max();
  short b = std::numeric_limits<short>::max();
  auto c = a + b;
  std::cout << "type: " << typeid(c).name() << "; value: " << c << std::endl;

  short d = a + b;
  std::cout << "type: " << typeid(d).name() << "; value: " << d << std::endl;

  float x = std::numeric_limits<float>::max();
  float y = std::numeric_limits<float>::max();
  auto z = 2.*x;
  std::cout << "type: " << typeid(z).name() << "; value: " << z << std::endl;
}

EXAMPLE_FUNCTION(numbers, 0)
{
  uint32_t x1 = 0b1111'0000'1111'0000'1111'0000'1111'0000;
  uint32_t x2 = 0xf0f0f0f0;
  uint32_t x3 = 4'042'322'160;

  assert(x1 == x2);
  assert(x1 == x3);
}

EXAMPLE_FUNCTION(bool_type, 0)
{
  bool a = 0, b = 2;
  bool c = a - b;
  std::cout << c << std::endl;
}

inline float ramp(float x)
{
  return (x > 0) * x;
}

EXAMPLE_FUNCTION(bool_type_cast, 0)
{
  int z = 3 < 5; // z == 1
  assert(z == 1);
  z += bool(6);
  assert(z == 2);
  z *= sizeof(char) < 7;
  assert(z == 2);

  assert(ramp(5) == 5);
  assert(ramp(-3) == 0);
}
