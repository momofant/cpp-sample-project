#include <set>
#include <map>
#include <string>
#include <string_view>
#include <ranges>
#include <iostream>
#include <unordered_set>
#include "examples.h"

EXAMPLE_FUNCTION(set_1, 1)
{
  struct A {
    inline bool operator < (A const& other) const {
      return true;
    }
  };
  std::set<A> s;
  s.insert(A{});
}


namespace set_2
{    
  struct A { };

  struct ACmp {
    inline bool operator () (A const& a, A const& b) const {
      return true;
    }
  };

  EXAMPLE_FUNCTION(set_2, 1)
  {
    std::set<A, ACmp> s;
    s.insert(A{});
  }
}

namespace set_3
{    
  struct A { };

  inline bool operator < (A const& a, A const& b) {
    return true;
  }

  EXAMPLE_FUNCTION(set_3, 1)
  {
    std::set<A> s;
    s.insert(A{});
  }
}

EXAMPLE_FUNCTION(map, 1)
{
  std::map<std::string_view, int> bin;
  bin["apple"] = 10;
  bin["orange"] = 12;
  bin["lime"] = 3;

  bin["lime"] += 1;

  for (auto e : bin)
    std::cout << e.first << " " << e.second << std::endl;

  auto i = bin.find("egg");
  if (i != bin.end())
    std::cout << i->first << " " << i->second << std::endl;
  else
    std::cout << "egg is out of stock" << std::endl;
}

EXAMPLE_FUNCTION(map_2, 1)
{
  std::map<std::string_view, int> bin = {
    {"apple", 0},
    {"lime", 0},
    {"orange", 1},
    {"orange", 0}
  };

  for (auto e : bin)
    std::cout << e.first << " " << e.second << std::endl;

  auto i = bin.find("egg");
  if (i != bin.end())
    std::cout << i->first << " " << i->second << std::endl;
  else
    std::cout << "egg is out of stock" << std::endl;
}

EXAMPLE_FUNCTION(unordered_set, 1)
{
  std::unordered_set<std::string_view> s1 {"abc", "bcd", "cde", "def"};
  std::set<std::string_view> s2 {"abc", "bcd", "cde", "def"};

  for (auto e : s1)
    std::cout << e << " ";
  std::cout << std::endl;

  for (auto e : s2)
    std::cout << e << " ";
  std::cout << std::endl;
}

EXAMPLE_FUNCTION(unordered_set_2, 1)
{
  struct A {
    int x, y;
  };
  struct AHash {
    size_t operator() (A const& a) {
      return a.x * 10 + a.y;
    }
  };
  std::unordered_set<A, AHash> s;
}
