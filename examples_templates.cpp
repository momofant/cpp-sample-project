#include <iostream>
#include <any>
#include <assert.h>
#include <complex>
#include <locale>
#include <fstream>
#include <codecvt>
#include <algorithm>
#include <numeric>
#include <cctype>
#include <string>
#include <memory>
#include <string.h>
#include <array>
#include <ranges>
#include "examples.h"


template <typename T>
inline T ramp(T const& x)
{
  return x < 0 ? 0 : x;
}

// inline int ramp(int x)
// {
//   return x < 0 ? 0 : x;
// }

// inline double ramp(double x)
// {
//   return x < 0 ? 0 : x;
// }

#define RAMP(x) ((x) < 0 ? 0 : (x))

EXAMPLE_FUNCTION(rampfunc_preprocessor, 0)
{
  double x = ramp(-6.);
  int y = ramp(-6);
  long z = RAMP(-6);

  // неочевидное поведение
  int v = 1;
  v = RAMP(--v);
  assert(v == -1);
}

template <typename To, typename From>
inline To convert(From const& from)
{
  return static_cast<To>(from);
}

EXAMPLE_FUNCTION(convert_types, 0)
{
  auto z = convert<int>(1.f);
  static_assert(std::is_same<decltype(z), int>::value);
}

template <class A, class B>
void add(A const& a, B const& b, decltype(a + b)& c)
{
  c = a + b;
}

template <class A, class B>
auto add(A const& a, B const& b)
{
  return a + b;
}

EXAMPLE_FUNCTION(several_types, 0)
{
  float x;
  add(1.f, 2l, x); // assumed add<float, long, float>
  x = add(1, 2); // assumed float add<float, int>
}

template <class T, int N>
struct Array
{
  T data[N] {};
};

template <class T, int N>
inline Array<T, N> gen_seq()
{
  Array<T, N> arr;
  for (int i = 0; i < N; ++ i)
    arr.data[i] = T(i);
  return arr;
}

template <class T, int N>
constexpr inline Array<T, N> gen_fib()
{
  Array<T, N> arr;
  if (N > 0)
    arr.data[0] = T(0);
  if (N > 1)
    arr.data[1] = T(1);
  for (int i = 2; i < N; ++ i)
    arr.data[i] = arr.data[i-1] + arr.data[i-2];
  return arr;
}

EXAMPLE_FUNCTION(template_struct, 0)
{
  Array<float, 3> arrf {1.f,2.f,3.f};
  Array<int, 3> arri {1,2,3};

  auto a = gen_seq<int, 5>();
  auto b = gen_seq<float, 10>();
  constexpr static auto c = gen_fib<int, 20>();
  constexpr static auto d = gen_fib<double, 100>();
}

namespace template_constexpr_fun
{
  template <typename T, int N>
  constexpr T maxelem(T const(&arr)[N])
  {
    static_assert(N > 0);
    T val = arr[0];
    for (int i = 1; i < N; ++ i)
      val = std::max(val, arr[i]);
    return val;
  }

  template <typename T, int N>
  struct Array {
    T _data[N];
  };

  EXAMPLE_FUNCTION(constexpr_fun, 0)
  {
    constexpr int sz = maxelem({1,2,3});
    Array<float, sz> arr;
  }
}

namespace gen_seq_v1
{
  template <unsigned long ... Args>
  struct sequence {};

  template <unsigned long First, unsigned long ... Tail>
  struct generate {
    using type = typename generate<First-1, First, Tail...>::type;
  };

  template <unsigned long ... Tail>
  struct generate<0, Tail...> {
    using type = sequence<Tail...>;
  };

  template <unsigned long N>
  using sequence_t = typename generate<N>::type;

  template <unsigned long ... Indices>
  std::array<unsigned long, sizeof...(Indices)> gen_seq(sequence<Indices...>) {
    return std::array<unsigned long, sizeof...(Indices)>{Indices...};
  }

  EXAMPLE_FUNCTION(gen_seq_v1, 0)
  {
    auto a = gen_seq(sequence_t<10>{});
    for (auto e : a) {
      std::cout << e << std::endl;
    }
  }
}

namespace gen_seq_v2
{
  template <unsigned long First, unsigned long ... Tail>
  struct generate : public generate<First-1, First, Tail...> {};

  template <unsigned long ... Values>
  struct generate<0, Values...> {
    constexpr static std::array<int, sizeof...(Values)> values = { Values... };
  };

  EXAMPLE_FUNCTION(gen_seq_v2, 0)
  {
    for (auto e : generate<10>::values) {
      std::cout << e << std::endl;
    }
  }
}

namespace gen_prime_numbers_v2
{
  template <unsigned long N, unsigned long Iterator, bool Continue>
  struct __is_prime {
    static const bool value = (N % Iterator != 0) && 
        __is_prime<N, Iterator+1, (N/Iterator >= Iterator)>::value;
  };

  template <unsigned long N, unsigned long Iterator> 
  struct __is_prime<N, Iterator, false> {
    static const bool value = true;
  };

  template <unsigned long N>
  struct is_prime : __is_prime<N, 2, (N/2 >= 2)> {};

  template <unsigned long N, bool P=is_prime<N>::value>
  struct next_prime : next_prime<N+1> {};

  template <unsigned long N>
  struct next_prime<N,true> {
    static const unsigned long value = N;
  };

  template <unsigned long K, unsigned long First, unsigned long ... Tail>
  struct generator : generator <K - 1, next_prime<First+1>::value, First, Tail...> {};

  template <unsigned long First, unsigned long ... Args>
  struct generator<0, First, Args...> {
    constexpr static unsigned long value[sizeof...(Args) + 1] = {First, Args...};
  };

  EXAMPLE_FUNCTION(gen_prime_numbers_v2, 0)
  {
    for (auto e : generator<100, 1>::value) {
      std::cout << e << std::endl;
    }
  }
}

namespace gen_prime_numbers_v3
{
  template <unsigned long N, unsigned long Iterator, bool Continue>
  struct __is_prime {
    static const bool value = (N % Iterator != 0) && 
        __is_prime<N, Iterator+1, (N/Iterator >= Iterator)>::value;
  };

  template <unsigned long N, unsigned long Iterator> 
  struct __is_prime<N, Iterator, false> {
    static const bool value = N > 1;
  };

  template <unsigned long N>
  struct is_prime : __is_prime<N, 2, (N/2 >= 2)> {};

  template <unsigned long N, bool P=is_prime<N>::value>
  struct next_prime : next_prime<N+1> {};

  template <unsigned long N>
  struct next_prime<N,true> {
    static const unsigned long value = N;
  };

  template <unsigned long K, unsigned long Actual=2, unsigned long ... Array>
  struct prime_generator : prime_generator <K - 1, next_prime<Actual+1>::value, Array..., Actual> {};

  template <unsigned long Actual, unsigned long ... Array>
  struct prime_generator<0, Actual, Array...> {
    constexpr static unsigned long value[sizeof...(Array)] = {Array...};
  };

  EXAMPLE_FUNCTION(gen_prime_numbers_v3, 0)
  {
    for (auto e : prime_generator<100>::value) {
      std::cout << e << std::endl;
    }
  }
}

namespace gen_prime_numbers_v4
{
  constexpr bool is_prime(unsigned long value)
  {
    for (int i = 2; i <= value / i; ++ i)
    {
      if (value % i == 0)
        return false;
    }
    return true;
  }

  template <unsigned long N>
  constexpr std::array<unsigned long, N> gen_primes()
  {
    static_assert(N > 0);

    unsigned long value = 2;
    int nfound = 1;
    std::array<unsigned long, N> primes = {value};
    ++ value;

    while (nfound < N)
    {
      if (is_prime(value))
      {
        primes[nfound] = value;
        ++ nfound;
      }
      ++ value;
    }

    return primes;
  }

  EXAMPLE_FUNCTION(gen_prime_numbers_v4, 0)
  {
    constexpr static auto primes = gen_primes<2000>();

    for (auto e : primes)
      std::cout << e << std::endl;
  }
}

EXAMPLE_FUNCTION(array_demo, 0)
{
  std::array<int, 3> a = {1,2,3};

  for (auto const& e : a)
    std::cout << e << std::endl;

  for (int i = 0; i < a.size(); ++ i)
    std::cout << a[i] << std::endl;
}

namespace template_specialization
{
  template <typename T>
  struct is_void {
    static const bool value = false;
  };
  template <>
  struct is_void<void> {
    static const bool value = true;
  };

  static_assert(is_void<int>::value == false);
  static_assert(is_void<void>::value == true);

  struct A { int a; };

  template <typename T>
  void print(T const& v) {
    std::cout << v << std::endl;
  }

  template <>
  void print<A>(A const& v) {
    std::cout << "A { " << v.a << " }" << std::endl;
  }

  template <typename T, std::size_t N>
  void print(std::array<T,N> const& arr) {
    for (auto const& e : arr) {
      std::cout << e << ' ';
    };
    std::cout << std::endl;
  }

  // xxx
  EXAMPLE_FUNCTION(template_specialization_1, 0)
  {
    print("hello");
    print(A{}); // ошибка
    print(std::array<int,3>{1,2,3});
  }

  template <typename T>
  struct complex {
    T x, y;

    T norm() const {
      return sqrt(x*x + y*y);
    }
  };

  template <>
  int complex<int>::norm() const = delete;

  EXAMPLE_FUNCTION(template_specialization_3, 0)
  {
    complex<double> d {1, 2};
    d.norm(); // ок

    complex<int> i {1, 2};
    // i.norm(); // ошибка
  }

  template <unsigned N>
  struct factorial {
    static const unsigned value = N * factorial<N-1>::value;
  };
  template <>
  struct factorial<0> {
    static const unsigned value = 1;
  };

  static_assert(factorial<5>::value == 120);
}

namespace template_specialization_4
{
  template <typename T>
  struct complex {
    T x, y;
  };

  template <typename T, typename S>
  auto operator + (complex<T> const& a, complex<S> const& b) {
    using D = decltype(a.x + b.x);
    return complex<D> {a.x + b.x, a.y + b.y};
  }

  EXAMPLE_FUNCTION(template_specialization_4, 0)
  {
    complex<double> a {1,2};
    complex<int> b {3,5};
    auto c = a + b;

    static_assert(
      std::is_same<decltype(c), complex<double>>::value
    );
  }

}

namespace constexpr_tests
{
  struct A {
    constexpr static int x = 1;
    const static int y = 1;
  };

  int f(int x) { return x+1; }
  const static int z = f(2); // ок

  constexpr int g(int x) { return x+1; }
  constexpr static int w = g(2); // ок
  // constexpr static int w = f(2); // ошибка
}

namespace saturate_cast_example
{
  template <typename To, typename From>
  constexpr inline To saturate_cast(From const& value) {
    return std::clamp<From>(value, 
      std::numeric_limits<To>::min(), 
      std::numeric_limits<To>::max()
    );
  }

  EXAMPLE_FUNCTION(saturate_cast_example, 0)
  {
    constexpr auto c = saturate_cast<char>(200);
    static_assert(c == 127);
  }
}

namespace variadic_template 
{
  template <typename Arg>
  inline void print(Arg const& arg)
  {
    std::cout << arg << std::endl;
  }

  template <typename First, typename ... Tail>
  inline void print(First const& first, Tail const& ... tail)
  {
    std::cout << first << ", ";
    print(tail...);
  }

  EXAMPLE_FUNCTION(variadic_template_0, 0)
  {
    print(1, 3., std::complex{1,4});
  }

  template <int ... Values>
  struct FillArray {
    constexpr static int value[] = {Values...};
  };

  EXAMPLE_FUNCTION(variadic_template_3, 0)
  {
    FillArray<1,3,1,4,6,23>::value;
    static_assert(sizeof(FillArray<1,3,1,4,6,23>) == 1);
  }

  template <typename T, int N, int ... Dims>
  struct MultiDimArrayType : MultiDimArrayType <T, Dims...> {
    using type = typename MultiDimArrayType<T, Dims...>::type[N];
  };

  template <typename T, int N>
  struct MultiDimArrayType<T, N> {
    using type = T[N];
  };

  template <typename S>
  inline auto& get_elem(S& arr, int i) {
    return arr[i];
  }

  template <typename S, typename ... Indices>
  inline auto& get_elem(S& arr, int i, Indices ... indices) {
    return get_elem(arr[i], indices...);
  }

  template <typename T, int ... Dims>
  struct MultiDimArray {
    using type = typename MultiDimArrayType<T, Dims...>::type;
    type data;

    template <typename ... Indices>
    T& at(Indices ... indices) {
      return get_elem(data, indices...);
    }

    template <typename ... Indices>
    T const& at(Indices ... indices) const {
      return get_elem(data, indices...);
    }
  };

  EXAMPLE_FUNCTION(variadic_template, 0)
  {
    MultiDimArray<float, 50, 100, 3> img;

    for (int y = 0; y < 50; ++ y)
    {
      for (int x = 0; x < 100; ++ x)
      {
        for (int c = 0; c < 3; ++ c)
          img.data[y][x][c] = y * 10000 + x * 100 + c;
      }
    }
    static_assert(sizeof(img) == 50 * 100 * 3 * sizeof(float));
    img.at(5,3,2) = 123456;
    assert(img.at(3,2,1) == 30201);
    assert(img.at(5,3,2) == 123456);
  }
}

namespace several_types_specialization
{
  template <typename T, bool Integral=std::is_integral<T>::value>
  struct S {
    constexpr static char s[] = "this is not integral";
  };

  template <typename T>
  struct S<T,true>  {
    constexpr static char s[] = "this is integral";
  };

  EXAMPLE_FUNCTION(several_types_specialization, 0)
  {
    S<int> x;
    S<double> y;
    std::cout << x.s << "|" << y.s << std::endl;
  }
}

namespace several_types_specialization_v2
{
  template <typename T, typename E=void>
  struct S {
    constexpr static char s[] = "this is not arithmetic";
  };

  template <typename T>
  struct S<T, typename std::enable_if_t<std::is_arithmetic<T>::value>>  {
    // для арифметических типов будет void=void
    // для неарифметических будет void = Not defined type
    // этой специализации не удовлетворяет
    constexpr static char s[] = "this is arithmetic";
  };

  EXAMPLE_FUNCTION(several_types_specialization_v2, 0)
  {
    S<int> x;
    S<double> y;
    S<std::complex<float>> z;
    std::cout << x.s << "|" << y.s << "|" << z.s << std::endl;
  }
}

//namespace several_types_specialization_v3
//{
//  template <typename T>
//  std::enable_if_t<!std::is_literal_type<T>::value, int> f() { return 0; }
//
//  template <typename T>
//  std::enable_if_t<std::is_literal_type<T>::value, int> f() { return 1; }
//
//  EXAMPLE_FUNCTION(several_types_specialization_v3, 0)
//  {
//    std::cout << f<std::any>() << std::endl;
//    std::cout << f<int>() << std::endl;
//  }
//
//}

EXAMPLE_FUNCTION(foreach_demo, 0)
{
  int arr[] = {1,2, 3};
  for (auto e : arr)
    std::cout << e << " ";
  std::cout << std::endl;
}

// template< class T >
// struct is_literal_type;

// struct A {
//    int m;
// };
// struct B {
//    virtual ~B();
// };
// static_assert(std::is_literal_type<A>::value);
// static_assert(!std::is_literal_type<B>::value);

//namespace partial_template_specialization
//{
//  template <typename T, typename S>
//  struct A { int x = 5; };
//
//  template <typename T>
//  struct A<T, T> { int x = 10; };
//
//  EXAMPLE_FUNCTION(partial_template_specialization, 0)
//  {
//    A<int,int> a;
//    A<int,float> b;
//
//    assert(a.x == 10);
//    assert(b.x == 5);
//  }
//
//  template <typename T>
//  struct B {
//    static void f() { std::cout << "not vector" << std::endl; }
//  };
//
//  template <typename T>
//  struct B<std::vector<T>> {
//    static void f() { std::cout << "vector" << std::endl; }
//  };
//
//  EXAMPLE_FUNCTION(partial_template_specialization_2, 0)
//  {
//    B<char>::f();
//    B<std::vector<float>>::f();
//  }
//
//  template <
//    typename T,
//    bool L=std::is_literal_type<T>::value,
//    bool A=std::is_arithmetic<T>::value,
//    bool I=std::is_integral<T>::value
//  >
//  struct C {
//    constexpr static char s[] = "other type";
//  };
//
//  template <typename T>
//  struct C<T,true,false,false> {
//    constexpr static char s[] = "literal";
//  };
//
//  template <typename T>
//  struct C<T,true,true,false> {
//    constexpr static char s[] = "arithmetic";
//  };
//
//  template <typename T>
//  struct C<T,true,true,true> {
//    constexpr static char s[] = "integral";
//  };
//
//  EXAMPLE_FUNCTION(type_cast, 0)
//  {
//    std::cout << "int: " << C<int>::s << std::endl;
//    std::cout << "float: " << C<float>::s << std::endl;
//    std::cout << "std::array<float,1>: " << C<std::array<float,1>>::s << std::endl;
//    std::cout << "std::string: " << C<std::string>::s << std::endl;
//  }
//}


namespace concept_demo
{
  template <typename T>
  struct C {
    constexpr static char s[] = "other type";
  };

  template <typename T>
  concept IntegralType = std::is_integral<T>::value;

  template <IntegralType T>
  struct C<T> {
    constexpr static char s[] = "integral";
  };

  EXAMPLE_FUNCTION(concept_demo, 0)
  {
    C<std::complex<float>> literal;
    std::cout << literal.s << std::endl;

    C<short> integral;
    std::cout << integral.s << std::endl;
  }

  template <typename T>
  struct S {
    constexpr static char s[] = "other";
  };

  template <typename T> requires std::is_integral<T>::value
  struct S<T> {
    constexpr static char s[] = "integral";
  };

  template <typename T> requires (std::is_arithmetic<T>::value && !std::is_integral<T>::value)
  struct S<T> {
    constexpr static char s[] = "arithmetic";
  };

  EXAMPLE_FUNCTION(concept_demo_2, 0)
  {
    std::cout << S<int>::s << std::endl;
    std::cout << S<float>::s << std::endl;
    std::cout << S<void>::s << std::endl;
  }


  template <typename T>
  concept Arithmetic = std::is_arithmetic<T>::value && !std::is_integral<T>::value;

  template <typename T>
  concept Integral = std::is_integral<T>::value;

  std::string f(Arithmetic auto a) { return "arithmetic"; }
  std::string f(Integral auto a) { return "integral"; }
  std::string f(auto a) { return "other"; }

  EXAMPLE_FUNCTION(concept_demo_3, 0)
  {
    std::cout << f(float{}) << std::endl;
    std::cout << f(int{}) << std::endl;
    std::cout << f(std::string{}) << std::endl;
  }

  template <typename T>
  std::string g(T a) { return "other"; }

  template <typename T>
  std::string g(T a)
    requires (std::is_arithmetic<T>::value && !std::is_integral<T>::value)
  {
    return "arithmetic";
  }

  template <typename T>
  std::string g(T a)
    requires (std::is_integral<T>::value)
  {
    return "integral";
  }

  EXAMPLE_FUNCTION(concept_demo_4, 0)
  {
    std::cout << g(float{}) << std::endl;
    std::cout << g(int{}) << std::endl;
    std::cout << g(std::string{}) << std::endl;
  }
}

namespace template_variable
{
  struct A{
    template <typename T>
    constexpr static T a = 1;
  };

  template <class T>
  inline const T pi = T(3.1415926535897932385L);

  EXAMPLE_FUNCTION(template_variable, 0)
  {
    const auto x = pi<int>; // 3
    // const auto z = pi<std::complex>;
    const auto z = pi<std::complex<double>>;

    A a;
    a.a<int>;
  }
}

namespace function_overloading
{
  // template function
  template <typename T>
  int f(T) { return 0; }

  // specialization
  template <>
  int f<int>(int) { return 5; }

  // overloading
  int f(int) { return 5; }

  // partial specialization
  // error: function template partial specialization is not allowed
  // template <typename T>
  // int f<std::vector<T>>(std::vector<T>) { return 10; }

  // overloading
  template <typename T>
  int f(std::vector<T>) { return 10; }
 
  EXAMPLE_FUNCTION(function_overloading, 0)
  {
    assert(f(1l) == 0);
    assert(f(1) == 5);
  }
}

namespace template_parameters
{
  struct X { int value; };
  template <X x>
  struct A {};

  template <int const* x>
  struct B {};

  template <auto c>
  struct C {};

  template <char const* s>
  struct D {};

  EXAMPLE_FUNCTION(template_parameters_1, 0)
  {
    A<X{1}> a;
    static const int z = 3;
    B<&z> b;
    C<10> c1;
    C<'1'> c2;
    C<10ll> c3;
    static const char s[] = "hello";
    D<s> d;
  }

  template <int (&r)[10]>
  struct W {};

  int a[10];

  EXAMPLE_FUNCTION(template_parameters_2, 0)
  {
    W<a> w;
  }

  template <void (*PF)(void)>
  void g() {
    std::cout << "g()" << std::endl;
    PF();
  }
  void h() {
    std::cout << "h()" << std::endl;
  }
  void u() {
    std::cout << "u()" << std::endl;
  }
  EXAMPLE_FUNCTION(template_parameters_3, 0)
  {
    g<h>();
    g<u>();
  }

}

EXAMPLE_FUNCTION(vector_demo, 0)
{
  std::vector<int> v {1,2,3};
  v.push_back(5);
  v.pop_back();
  v.push_back(4);
  v.push_back(5);

  auto const* pdata = &v.at(0);
  for (int i = 0; i < v.size(); ++ i)
    std::cout << pdata[i] << ' ';
  std::cout << std::endl;
}

EXAMPLE_FUNCTION(initializer_list_demo, 0)
{
  struct S { int x, y; };
  S s {3, 5};

  class A {
    int x, y;
  public:
    A(std::initializer_list<int> l) { /* ... */ }
  };
  A a {3, 5};
}

EXAMPLE_FUNCTION(string_cvt, 0)
{
  std::string s = "привет!";
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt;
  std::u16string w = cvt.from_bytes(s);

  std::cout << s.size() << std::endl; // 13 symbols of char
  std::cout << w.size() << std::endl; // 7 symbols of char16_t
}

EXAMPLE_FUNCTION(new_operator, 0)
{
  struct X {
    int z;
    X() { std::cout << "X" << std::endl; }
    ~X() { std::cout << "~X" << std::endl; }
  };
  char data[10];
  X* p2 = reinterpret_cast<X*>(data);
  new (p2) X;
  p2->~X();

  new (p2) X[2];
  p2[0].~X();
  p2[1].~X();
}

EXAMPLE_FUNCTION(new_operator_overloading, 0)
{
  static char buffer[100];
  static void* ptr = buffer;

  struct X {
    static void* operator new(std::size_t sz) {
      std::cout << "custom new for size " << sz << '\n';
      return ptr;
    }
    static void* operator new(std::size_t sz, void* p) {
      std::cout << "custom new with given pointer for size " << sz << '\n';
      return ptr;
    }
    static void* operator new[](std::size_t sz) {
      std::cout << "custom new[] for size " << sz << '\n';
      return ptr;
    }
    static void* operator new[](std::size_t sz, void* p) {
      std::cout << "custom new with given pointer for size " << sz << '\n';
      return ptr;
    }
    static void operator delete(void* ptr, std::size_t sz) {
      std::cout << "custom delete for size " << sz << '\n';
    }
    static void operator delete[](void* ptr, std::size_t sz) {
      std::cout << "custom delete for size " << sz << '\n';
    }
  };

  auto* p = new X;
  delete p;

  auto* q = new X[3];
  delete[] q;

  new (p) X;

  struct A {
    static void* operator new(std::size_t sz, void* p, int a, char b, float c) {
      return p;
    }
  };

  new (nullptr, 1, 'b', 1.f) A;
}


namespace iterator_demo
{
  template <typename T>
  inline void copy(T* ptr) {}

  template <typename T, typename First, typename ... Values>
  inline void copy(T* ptr, First const& first, Values const&... values) {
    *ptr = first;
    copy(ptr + 1, values...);
  }

  template <int D1, int D2>
  class Array2D {
    int _arr[D1][D2];
  public:
    template <typename ... Args>
    void set(Args... args) {
      static_assert(sizeof...(Args) == D1 * D2);
      copy(&_arr[0][0], args...);
    }

    class iterator_const {
      int const* _ptr;
    public:
      iterator_const() = delete;
      explicit iterator_const(int const* ptr) : _ptr(ptr) {}
      inline iterator_const& operator ++ () {
        ++ _ptr;
        return *this;
      }
      inline iterator_const& operator -- () {
        -- _ptr;
        return *this;
      }
      inline bool operator == (iterator_const const& other) const { return _ptr == other._ptr; }
      inline bool operator != (iterator_const const& other) const { return _ptr != other._ptr; }
      inline int const* operator -> () const { return _ptr; }
      inline int const& operator * () const { return *_ptr; }
    };

    inline iterator_const begin() const { return iterator_const(&_arr[0][0]); };
    inline iterator_const end() const { return iterator_const(&_arr[D1-1][D2]); };
  }; 

  EXAMPLE_FUNCTION(iterator_demo, 0)
  {
    Array2D<3, 2> arr;
    arr.set(
      1,2,
      3,4,
      5,6
    );

    for (auto it = arr.begin(); it != arr.end(); ++ it)
      std::cout << *it << ' ';

  }
}

inline std::string toupper(std::string& s) {
  std::transform(s.begin(), s.end(), s.begin(), 
    [](unsigned char c){ return std::toupper(c); }
  );
  return s;
}

EXAMPLE_FUNCTION(stditerator_demo, 0)
{
  std::vector<std::string> fruits {
    "orange",
    "apple",
    "raspberry"
  };

  for (auto const& e : fruits)
    std::cout << e << std::endl;

  for (auto it = fruits.begin(); it != fruits.end(); ++ it)
    *it = toupper(*it);

  for (auto it = fruits.crbegin(); it != fruits.crend(); ++ it)
    std::cout << it->c_str() << std::endl;
}

std::tuple<bool, std::string> f(int arg)
{
  if (arg < 0)
    return std::make_tuple(false, "");
  return std::make_tuple(true, "ok");
}

EXAMPLE_FUNCTION(tuples, 0)
{
  std::tuple<int, float> t {1,2.f};
  auto v = std::make_tuple(1, 2.f);
  auto w = std::make_tuple(4, 1.f);
  auto s = std::make_tuple(4, 1.f, 'a');
  assert(t == v);
  assert(w > v);
  // assert(s != w); //

  auto [t1,t2] = t;
  std::tie(t1, std::ignore) = t;

  std::get<1>(t) = 5.f;
}

template<typename T>
inline constexpr bool always_false_v = false;

template <typename T>
auto round_(T const& a) {
  if constexpr (std::is_same_v<T, float>) {
    return a > 0 ? int(a + 0.5f) : int(a - 0.5f);
  }
  else if constexpr (std::is_same_v<T, double>) {
    return a > 0 ? int64_t(a + 0.5) : int64_t(a - 0.5);
  }
  else if constexpr (std::is_integral_v<T>) {
    return a;
  }
  else {
    static_assert(always_false_v<T>, "not implemented for this type");
  }
}

EXAMPLE_FUNCTION(if_constexpr_1, 0)
{
  auto a = round_(4);
  std::cout << a << std::endl;
  auto b = round_(3.5);
  std::cout << b << std::endl;
}

template <typename>
constexpr bool is_float = false;

template <>
constexpr bool is_float<float> = true;

EXAMPLE_FUNCTION(template_variable_3, 0)
{
  static_assert(is_float<int> == false);
  static_assert(is_float<float> == true);
}

template <typename T>
class smart_ptr
{
  T* _p = nullptr;
public:
  smart_ptr(smart_ptr const&) = delete;
  smart_ptr(T* p) : _p(p) {}
  ~smart_ptr() {
    if (_p) delete _p;
  }

  inline T* operator ->() { return _p; }
  inline T& operator *() { return *_p; }
};

EXAMPLE_FUNCTION(smartptr, 0)
{
  struct A{
    int x, y;
  };
  smart_ptr<A> p(new A {4,5});
  std::cout << p->x + p->y << std::endl;
}

EXAMPLE_FUNCTION(smartptr_2, 0)
{
  struct A{
    A() = delete;
    A(int a1, int a2) { std::cout << "A" << std::endl; }
    ~A() { std::cout << "~A" << std::endl; }
    A(A const&) { std::cout << "copy A" << std::endl; }
    A(A&&) { std::cout << "move A" << std::endl; }
  };

  A a(1,2);
  std::vector<A> v;
  // v.push_back(A{1,2});
  // v.emplace_back(A{1,2});
  // a is no valid after this

  // std::cout << "xxx" << std::endl;
  v.emplace_back(1,2);
  // v.push_back(A{});
  // v.emplace_back();

  std::vector<std::unique_ptr<A>> arr;
  arr.emplace_back(new A{1,2});
  // arr.push_back(new A{1,2}); // ошибка
  std::unique_ptr<A> p(new A{3,4});
  // arr[0] = p; // ошибка
  arr[0] = std::move(p);

  auto q = std::make_unique<A>(1,2);
}


EXAMPLE_FUNCTION(smartptr_3, 0)
{
  struct B;

  struct A {
    std::shared_ptr<B> b;
    A() = default;
    A(std::shared_ptr<B> b) : b(b) {}
    ~A() { std::cout << "~A" << std::endl; }
  };

  struct B {
    std::weak_ptr<A> a;
    B() = default;
    B(std::shared_ptr<A> a) : a(a) {}
    ~B() { std::cout << "~B" << std::endl; }
  };

  auto a = std::make_shared<A>();
  auto b = std::make_shared<B>(a);
  a->b = b;
}


EXAMPLE_FUNCTION(unique_ptr_cast, 0)
{
  struct A {
    int a = 1;
  };
  struct B : A {
    int b = 2;
  };
  auto pb = std::make_unique<B>();
  std::unique_ptr<A> pa = std::move(pb);
  assert(pa);
  assert(!pb);
  std::unique_ptr<B> pb2(static_cast<B*>(pa.release()));
  assert(!pa);
  assert(pb2);
}

EXAMPLE_FUNCTION(shared_ptr_cast, 0)
{
  struct A {
    virtual ~A() {}
  };
  struct B : A {};
  struct C : A {};
  std::shared_ptr<B> pb = std::make_shared<B>();
  std::shared_ptr<A> pa = pb;
  assert(pa);
  assert(pb);
  std::shared_ptr<B> pb2 = std::static_pointer_cast<B>(pa);
  std::weak_ptr<B> pb3 = std::static_pointer_cast<B>(pa);
  assert(pb2);
  std::shared_ptr<B> pb4 = pb3.lock();
  assert(pb4);
  std::shared_ptr<B> pb5 = std::dynamic_pointer_cast<B>(pa);
  assert(pb5);
  std::shared_ptr<C> pc = std::dynamic_pointer_cast<C>(pa);
  assert(!pc);
}

namespace shared_ptr_cast_2 
{
  void f(std::shared_ptr<std::string const> s) { }
  void g(std::shared_ptr<std::string>& s) { }
  void h(std::string* s) { }

  EXAMPLE_FUNCTION(shared_ptr_cast_2, 0)
  {
    std::shared_ptr<std::string> s = std::make_shared<std::string>("123");
    f(s);
    g(s);
    h(s.get());
  }
}

// template <typename TLoggerImpl>
//   requires requires(TLoggerImpl logger) {
//     logger.LogDebug(std::string_view{});
//     logger.LogInfo(std::string_view{});
//     logger.LogError(std::string_view{});
//   }
// struct Logger {};


template <typename Impl>
class Logger {
public:
  void debug(std::string const& s) {
    static_cast<Impl*>(this)->write("[debug]" + s);
  }
  void warn(std::string const& s) {
    static_cast<Impl*>(this)->write("[warn]" + s);
  }
};

class ScreenLogger : public Logger<ScreenLogger> {
public:
  void write(std::string const& s) {
    std::cout << s << std::endl;
  }
};

class FileLogger : public Logger<FileLogger> {
  std::ofstream f;
public:
  void write(std::string const& s) {
    f << s << std::endl;
  }
  FileLogger() {
    f.open("/tmp/log.txt");
  }
};

EXAMPLE_FUNCTION(static_polymorphism_4, 0)
{
  ScreenLogger logger;
  logger.debug("this is a debug message");

  FileLogger logger2;
  logger2.warn("this is a warning message");
}


inline bool is_prime(int value) {
  if (value < 2)
    return false;
  for (int i = 2; i * i <= value; ++ i)
    if (value % i == 0)
      return false;
    return true;
}

struct prime_generator {
  struct iterator {
    int value = 2;
    int index = 0;

    iterator() {}
    iterator(int initial_value) {
      value = initial_value;
      while (!is_prime(value)) { ++ value; }
    }
    iterator(int initial_value, int index) : 
      value(initial_value), index(index) {}
    inline void next() {
      int i = value + 1;
      while (!is_prime(i)) { ++ i; }
      value = i;
      ++ index;
    }
    inline iterator operator ++() {
      next();
      return *this;
    }
    inline bool operator != (iterator const& other) {
      return other.index != index;
    }
    inline int operator * () const {
      return value;
    }
  };

  int from;
  int n;

  prime_generator(int n=0, int from=2) : from(from), n(n) {}
  inline iterator begin() const {
    return iterator(from);
  }
  inline iterator end() const {
    iterator res(2, n);
    return res;
  }
};

EXAMPLE_FUNCTION(ranges_1, 0)
{
  for (auto e : prime_generator(10)) {
    std::cout << e << std::endl;
  }
}

struct odd_generator {
  struct iterator {
    int i = 0;
    iterator(int i) : i(i) {}
    inline iterator& operator ++ () {
      i += 2;
      return *this;
    }
    inline int operator * () const { return i; }
    inline bool operator == (iterator const& other) const {
      return i == other.i;
    }
  };

  int n, from;

  odd_generator(int n, int from = 1) {
    this->n = n;
    this->from = from + ((from + 1) % 2);
  }
  iterator begin() const {
    return iterator(from);
  }
  iterator end() const {
    return iterator(from + 2*n);
  }

};

EXAMPLE_FUNCTION(ranges_2, 0)
{
  for (auto e : odd_generator(5)) {
    std::cout << e << std::endl;
  }
}

EXAMPLE_FUNCTION(ranges_3, 0)
{
  // auto i = std::views::iota(1, 5);
}

/*
  template<typename _Tp>
    concept viewable_range = range<_Tp> && (borrowed_range<_Tp> || view<remove_cvref_t<_Tp>>);

	template<viewable_range _Range>
	  requires requires { declval<_Callable>()(declval<_Range>()); }
	  friend constexpr auto
	  operator|(_Range&& __r, const _RangeAdaptorClosure& __o)
	  { return __o(std::forward<_Range>(__r)); }
*/

// template <class ForwardIterator, class Generator>
//   void generate (ForwardIterator first, ForwardIterator last, Generator gen);

EXAMPLE_FUNCTION(stringview, 0)
{
  std::string_view s("abc");
  std::cout << s.size() << std::endl;
  std::string_view d("xyz");
  std::cout << (s < d) << std::endl;  
}

EXAMPLE_FUNCTION(stringview_2, 0)
{
  char buf[10] = "abc";
  std::string_view s(buf, 10);
}

namespace if_constexpr
{
  template <typename T>
  inline auto f(T a)
  {
    if constexpr (std::same_as<T, int>) {
      return 1;
    }
    else {
      return "a";
    }
  }

  EXAMPLE_FUNCTION(if_constexpr_2, 0)
  {
    auto x = f(1);
    auto y = f(5l);
  }
}

namespace concept_demo_2
{
  template <typename T>
  inline bool equal(T a, T b) {
    return a == b;
  }

  template <std::floating_point T>
  inline bool equal(T a, T b) {
    return std::abs(a - b) < 1e-12;
  }

  template <typename T>
  concept char_const_ptr = std::is_same_v<T, char const*>;

  template <char_const_ptr T>
  inline bool equal(T a, T b) {
    return !strcmp(a, b);
  }

  EXAMPLE_FUNCTION(concept_demo_5, 0)
  {
    std::cout << equal(1, 1) << std::endl;
    std::cout << equal(1., 1.) << std::endl;
    std::cout << equal("abc", "def") << std::endl;
    exit(-1);
  }
}
