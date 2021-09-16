#include <iostream>
#include <assert.h>
#include "anothefile.h"
#include "examples.h"


int twice(int arg) { return arg * 2; }
long twice(long arg) { return arg * 2; } // ok

int increment(int value)
{
    return value + 1;
}

inline void increment(int* value)
{
  ++ *value;
}


int callconter()
{
  static int n = 0;
  return n ++;
}

void example52()
{
  std::cout << callconter() << std::endl;
  std::cout << callconter() << std::endl;
  std::cout << callconter() << std::endl;
  std::cout << callconter() << std::endl;
}

void example53()
{
  somefunc();
  std::cout << &globconst << std::endl;
}


constexpr int max(int a, int b)
{
  return a > b ? a : b;
}

void example54()
{
  constexpr int sz = max(3, 5);
  char buf[sz];
}

constexpr int sum(int const* arr, int sz)
{
  int sum = 0;
  int const* pend = arr + sz;
  while (arr < pend)
  {
    sum += *arr;
    ++arr;
  }
  return sum;
}

void example55()
{
  constexpr const int arr[] = {1,2,3};
  constexpr int s = sum(arr, 3);
  std::cout << s << std::endl;
}

void reverse(int* arr, int sz)
{
  for (int i = 0; i < sz/2; ++ i)
  {
    std::swap(arr[i], arr[sz - 1 - i]);
  }
}

void example56()
{
  int arr[] = {1,3,5,7,9};
  int n = sizeof(arr) / sizeof(*arr);
  reverse(arr, n);

  for (int i = 0; i < n; ++ i)
    std::cout << arr[i] << " ";

  std::cout << std::endl;
}

/**
 * constant expressions
 */

inline constexpr int string_length(char const* s)
{
  int n = 0;
  while (s[n] != '\0')
    ++n;
  return n;
}

void example65()
{
  // the function works with constexpr arguments
  constexpr char const s[] = "abc";
  constexpr int ns = string_length(s);
  std::cout << ns << std::endl;
  // AWA with usual arguments
  char buf[10];
  std::cin.get(buf, sizeof(buf));
  int nb = string_length(buf);
  std::cout << nb << std::endl;
}

inline int sum(int a, int b=0, int c=0, int d=0, int e=0)
{
  return a + b + c + d + e;
}

EXAMPLE_FUNCTION(default_arguments, 0)
{
  assert(sum(1, 2) == 3);
  assert(sum(1, 2, 3, 4) == 10);
}

inline int f(int x)
{
  return x + 1;
}

inline int g(int x, int y = f(5))
{
  return x + y;
}

