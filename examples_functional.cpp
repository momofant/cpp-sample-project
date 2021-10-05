#include <iostream>
#include <assert.h>
#include <ranges>
#include <functional>
#include <algorithm>
#include <array>
#include "examples.h"


int f(int) { return 0; }

EXAMPLE_FUNCTION(function_types, 0)
{
  using F = int(int);
  using G = decltype(f);
  static_assert(std::is_same_v<F,G>);

  // F a;
  // a(1);

  std::cout << sizeof(std::function<int(int)>) << std::endl;
}

EXAMPLE_FUNCTION(lambda, 0)
{
  auto w = []() { return 5; };
  int x = w();
  // анонимная функция
  auto f = [x](int a) -> int { return x + a; };
  int y = f(3);
  assert(y == 8);
  x += 5;
  int z = f(5);
  assert(z == 10);

  // анонимная функция
  auto g = [&x](int a) -> int { return x + a; };
  auto m = [=]() { return f(x + y + z); }; // захватить все локальные переменные (по значению)
  auto n = [&]() { return g(x + y + z); }; // захватить все локальные переменные (по ссылке)

  int u = m();
  int v = n();
}

auto create_fun(int value) {
  auto f = [&value](int arg) {
    return arg + value;
  };
  return f;
}

EXAMPLE_FUNCTION(lambda_2, 0)
{
  auto f = create_fun(2);
  // assert(f(1) == 3); // ошибка

  struct A {
  private:
    void a() {}
  public:
    void b() {
      auto f = [this]() { a(); };
      f();
    }
  };

  A a;
  a.b();
  int x = 5;
  auto g = [px = &x]() { return px; };
  assert(g() == &x);


  [cx = std::as_const(x), nx = x + 1]() { return cx; };
}

EXAMPLE_FUNCTION(sort_example, 0)
{
  std::array<int, 10> arr;
  for (int i = 0; i < arr.size(); ++ i)
    arr[i] = std::rand() % 10;

  auto comp = [](int a, int b) { return a < b; };

  std::sort(arr.begin(), arr.end(), comp);
  for (int i = 0; i < arr.size(); ++ i)
    std::cout << arr[i] << " ";

  std::cout << std::endl;
}

EXAMPLE_FUNCTION(sort_example_2, 0)
{
  struct Person {
    std::string name;
    unsigned int age;
  };
  std::vector<Person> perons = {
    {"A", 10},
    {"B", 20},
    {"D", 55}
  };

  // struct ComparePersions {
  //   enum CompareBy {
  //     Age, Name
  //   };
  //   CompareBy compare_by;
  //   bool operator () (Person const& a, Person const& b) {
  //     return compare_by == Age ? a.age < b.age : 
  //       compare_by == Name ? a.name < b.name : false;
  //   }
  // };

  enum CompareBy {
    Age, Name
  };
  auto compare_by = Age;

  auto comp = 
    compare_by == Name ? [](Person const& a, Person const& b) {
      return a.name < b.name;
    } : 
    [](Person const& a, Person const& b) {
      return a.age < b.age;
    };

  std::sort(perons.begin(), perons.end(), comp);
}

void print_int(int val) {
  printf("%d ", val);
}

EXAMPLE_FUNCTION(functional_demo, 0)
{
  using namespace std;

  // Seq = [1,2,3,4,5,7,8],
  // Seq2 = lists:concat([
  //   lists:sublist(Seq, 5),
  //   [6],
  //   lists:sublist(Seq, 6, 2)
  // ]),

  // vector<int> arr = {1,2,3,4,5,7,8};
  // arr.insert(arr.begin() + 5, 6);
  // for (auto e : arr)
  //   cout << e << ' ';
  // cout << endl;

  const vector<int> arr = {1,2,3};
  vector<int> dst(arr.size());
  for (int i = 0; i < arr.size(); ++ i)
    dst[i] = arr[arr.size() - i - 1];

  for (auto e : dst)
    cout << e << ' ';
  cout << endl;

  // ranges::for_each(arr | views::reverse, print_int);
  // auto v1 = r | views::take(5);
  // auto v2 = r | views::drop(5);

  // std::ranges::merge(v1, v2);
  // for (auto e : v1) {
  //   std::cout << e << std::endl;
  // };
  // views::single_view(6);
  // for (auto e : v2 | views::reverse) {
  //   std::cout << e << std::endl;
  // };
  // auto j = {v1, v2};


  // for (auto e :  | views::join) {
  //   std::cout << e << std::endl;
  // };

  // for (auto e : r | views::take(4)) {
  //   std::cout << e;
  // }
  // std::ranges::subrange(arr.begin(), arr.begin() + 6);
  // auto rng = view::generate(arr) | view::take(5) | view::join;  
}

namespace {
  using namespace std;

  function<string(string)> message_generator(
        string const& prefix, string const& suffix) {
    return [prefix, suffix](string message) {
      return prefix + " " + message + " " + suffix;
    };
  }

  EXAMPLE_FUNCTION(closure_demo, 1) {
    auto generator = message_generator("Hello", "!");
    cout << generator("Max") << endl;
  }

}
