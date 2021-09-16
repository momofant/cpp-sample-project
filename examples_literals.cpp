#include <iostream>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "examples.h"

std::string operator "" _lower(char const* src, unsigned long n)
{
  std::string dst;
  dst.resize(n + 1);
  char* pdst = &dst[0];

  for (int i = 0; i < n; ++ i)
  {
    if (src[i] >= 'A' && src[i] <= 'Z')
    {
      pdst[i] = src[i] - 'A' + 'a';
    }
    else
    {
      pdst[i] = src[i];
    }
  }
  pdst[n] = '\0';
  return pdst;
}

EXAMPLE_FUNCTION(user_string_literals, 0)
{
  auto s = "AbCDefG"_lower;
  assert(s == "abcdefg");
}

EXAMPLE_FUNCTION(system_literals, 0)
{
  // int
  auto x1 = 10l;
  auto x2 = 10ul;
  auto x3 = 10ll;
  auto x4 = 10ull;
  auto x5 = 10;

  static_assert(std::is_same<decltype(x1), long>::value);
  static_assert(std::is_same<decltype(x2), unsigned long>::value);
  static_assert(std::is_same<decltype(x3), long long>::value);
  static_assert(std::is_same<decltype(x4), unsigned long long>::value);
  static_assert(std::is_same<decltype(x5), int>::value);

  // float
  auto y1 = 10.;
  auto y2 = 10.f;
  auto y3 = 10.l;

  static_assert(std::is_same<decltype(y1), double>::value);
  static_assert(std::is_same<decltype(y2), float>::value);
  static_assert(std::is_same<decltype(y3), long double>::value);

  // char
  auto c1 = 'x'; // char
  auto c2 = L'x'; // wchar_t
  auto c3 = U'x'; // char32_t
  auto c4 = u'x'; // char16_t
  auto c5 = u8'x'; // char8_t

  static_assert(std::is_same<decltype(c1), char>::value);
  static_assert(std::is_same<decltype(c2), wchar_t>::value);
  static_assert(std::is_same<decltype(c3), char32_t>::value);
  static_assert(std::is_same<decltype(c4), char16_t>::value);
  static_assert(std::is_same<decltype(c5), char8_t>::value);

  // string
  auto s1 = "abc";
  auto s2 = L"abc";
  auto s3 = u"abc";
  auto s4 = uR"(abc\n)";

  static_assert(std::is_same<decltype(s1), char const*>::value);
  static_assert(std::is_same<decltype(s2), wchar_t const*>::value);
  static_assert(std::is_same<decltype(s3), char16_t const*>::value);
  static_assert(std::is_same<decltype(s4), char16_t const*>::value);
}


struct complex
{
  double x, y;
  complex() = default;
  complex(double x) : x(x) {}
  complex(double x, double y) : x(x), y(y) {}
};

inline complex operator + (complex const& a, complex const& b)
{
  return {a.x + b.x, a.y + b.y};
}

inline complex operator * (complex const& a, complex const& b)
{
  return {a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x};
}


// literals
inline complex operator"" _i(long double im)
{
  return {0., double(im)};
}

inline complex operator"" _i(unsigned long long im)
{
  return {0., double(im)};
}

void print(complex const& z)
{
  std::cout << z.x << "+" << z.y << "i";
}

EXAMPLE_FUNCTION(user_numeric_literals, 0)
{
  complex z1 = 1. + 2_i;
  complex z2 = 5 + 3._i;
  complex z3 = z1 * z2;
  print(z3);
}


inline long double operator "" _pi (long double x)
{
  return x * M_PI;
}

inline long double operator "" _pi (unsigned long long x)
{
  return x * M_PI;
}

EXAMPLE_FUNCTION(pi_constant, 0)
{
  double x = 10.0_pi - 2_pi;
  assert(x == 8 * M_PI);
}
