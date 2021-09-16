#include <iostream>
#include "examples.h"


std::string operator "" _base64(const char* src, size_t n)
{
  char const table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string dst;
  dst.resize((n * 4 + 2) / 3 + 1);
  char* pdst = &dst[0];
  int i;

  for (i = 0; i < n - 2; i += 3)
  {
    uint8_t c0 = src[i];
    uint8_t c1 = src[i+1];
    uint8_t c2 = src[i+2];

    int a1 = (c0 >> 2) & 0b111111;
    int a2 = ((c0 << 4) | (c1 >> 4)) & 0b111111;
    int a3 = ((c1 << 2) | (c2 >> 6)) & 0b111111;
    int a4 = c2 & 0b111111;

    *pdst++ = table[a1];
    *pdst++ = table[a2];
    *pdst++ = table[a3];
    *pdst++ = table[a4];
  }

  if (i == n - 2)
  {
    uint8_t c0 = src[i];
    uint8_t c1 = src[i+1];

    int a1 = (c0 >> 2) & 0b111111;
    int a2 = ((c0 << 4) | (c1 >> 4)) & 0b111111;
    int a3 = (c1 << 2) & 0b111111;

    *pdst++ = table[a1];
    *pdst++ = table[a2];
    *pdst++ = table[a3];
  }
  else if (i == n - 1)
  {
    uint8_t c0 = src[i];
    int a1 = (c0 >> 2) & 0b111111;
    int a2 = (c0 << 4) & 0b111111;
    *pdst++ = table[a1];
    *pdst++ = table[a2];
  }

  *pdst = '\0';
  return dst;
}

std::string encode_base64(char const* src, int n)
{
  char const table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string dst;
  dst.resize((n * 4 + 2) / 3 + 1);
  char* pdst = &dst[0];
  int i;

  for (i = 0; i < n - 2; i += 3)
  {
    uint8_t c0 = src[i];
    uint8_t c1 = src[i+1];
    uint8_t c2 = src[i+2];

    int a1 = (c0 >> 2) & 0b111111;
    int a2 = ((c0 << 4) | (c1 >> 4)) & 0b111111;
    int a3 = ((c1 << 2) | (c2 >> 6)) & 0b111111;
    int a4 = c2 & 0b111111;

    *pdst++ = table[a1];
    *pdst++ = table[a2];
    *pdst++ = table[a3];
    *pdst++ = table[a4];
  }

  if (i == n - 2)
  {
    uint8_t c0 = src[i];
    uint8_t c1 = src[i+1];

    int a1 = (c0 >> 2) & 0b111111;
    int a2 = ((c0 << 4) | (c1 >> 4)) & 0b111111;
    int a3 = (c1 << 2) & 0b111111;

    *pdst++ = table[a1];
    *pdst++ = table[a2];
    *pdst++ = table[a3];
  }
  else if (i == n - 1)
  {
    uint8_t c0 = src[i];
    int a1 = (c0 >> 2) & 0b111111;
    int a2 = (c0 << 4) & 0b111111;
    *pdst++ = table[a1];
    *pdst++ = table[a2];
  }

  *pdst = '\0';
  return dst;
}

EXAMPLE_FUNCTION(base64_literals, 0)
{
  auto s = "light work."_base64;
  std::cout << typeid(s).name() << std::endl;
}

template <int n>
struct str
{
  char data[n];
};

template <int N, int M = (4*(N - 1) + 2) / 3 + 1>
constexpr str<M> base64(str<N> const& src)
{
  constexpr int n = N - 1;
  const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  char const* psrc = src.data;
  str<M> dst {""};

  char* pdst = dst.data;
  int i;

  for (i = 0; i < n - 2; i += 3)
  {
    uint8_t c0 = psrc[i];
    uint8_t c1 = psrc[i+1];
    uint8_t c2 = psrc[i+2];

    int a1 = (c0 >> 2) & 0b111111;
    int a2 = ((c0 << 4) | (c1 >> 4)) & 0b111111;
    int a3 = ((c1 << 2) | (c2 >> 6)) & 0b111111;
    int a4 = c2 & 0b111111;

    *pdst++ = table[a1];
    *pdst++ = table[a2];
    *pdst++ = table[a3];
    *pdst++ = table[a4];
  }

  if (i == n - 2)
  {
    uint8_t c0 = psrc[i];
    uint8_t c1 = psrc[i+1];

    int a1 = (c0 >> 2) & 0b111111;
    int a2 = ((c0 << 4) | (c1 >> 4)) & 0b111111;
    int a3 = (c1 << 2) & 0b111111;

    *pdst++ = table[a1];
    *pdst++ = table[a2];
    *pdst++ = table[a3];
  }
  else if (i == n - 1)
  {
    uint8_t c0 = psrc[i];
    int a1 = (c0 >> 2) & 0b111111;
    int a2 = (c0 << 4) & 0b111111;
    *pdst++ = table[a1];
    *pdst++ = table[a2];
  }

  *pdst = '\0';
  return dst;
}

EXAMPLE_FUNCTION(constexpr_test, 0)
{
  constexpr str<12> s { "light work." };
  constexpr auto d = base64(s);
  std::cout << d.data << std::endl;
}
