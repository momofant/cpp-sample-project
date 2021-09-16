#include <iostream>
#include <complex>
#include <assert.h>
#include <string.h>
#include <memory.h>
#include "examples.h"


EXAMPLE_FUNCTION(initialize_struct, 0)
{
  /**
   * различные способы инициализации переменной типа `struct`
   */
  struct S {
    int a;
    float b;
    char c[3];
  };

  // aggragate initialization
  S s1 {1, 2.f, "aa"};
  assert(s1.a = 1 && s1.b == 2.f && !memcmp(s1.c, "aa\0", sizeof(s1.c)));

  // aggragate initialization
  S s2 = {3};
  assert(s2.a = 3 && s2.b == 0 && !memcmp(s2.c, "\0\0\0", sizeof(s2.c)));

  // not initialized (garbage)
  S s3;

  // copy initialization
  S s4 = s1;
  assert(!memcmp(&s1, &s4, sizeof(s1)));

  // designate initialization
  S s5 {
      .a = 1,
      .c = "cc"
  };
  assert(s5.a = 1 && s5.b == 0 && !memcmp(s5.c, "cc\0", sizeof(s5.c)));

  // manually filled
  S s6;
  s6.a = 1;
  s6.b = 2.f;
  strcpy(s6.c, "dd");
  assert(s6.a = 1 && s6.b == 2.f && !memcmp(s6.c, "dd\0", sizeof(s6.c)));
}

struct Employee_ {
  char name[32];
  char surname[32];
  int age;
  char office[32];
  enum {
    Male, Female
  } sex;
};

char const* str(decltype(Employee_::Male) sex)
{
  return
    sex == Employee_::Male ? "male" :
    sex == Employee_::Female ? "female" : 
    "undefined";
}

void print(Employee_ employee)
{
  /**
   * Передача переменной по значению -- большие накладные расходы
   */
  std::cout
    << "name: " << employee.name << " " << employee.surname  << "\n"
    << "age: " << employee.age << "\n"
    << "office: " << employee.office << "\n"
    << "sex: " << str(employee.sex)  << std::endl;
}

EXAMPLE_FUNCTION(non_initialized_struct, 0)
{
  /**
   * Неинициализированные переменные могут доставлять проблемы
   */
  Employee_ employee1 {"Max", "Surov", 33, "11.3.21", Employee_::Male};
  Employee_ employee2;
  std::cout << "* employee 1:" << std::endl;
  print(employee1);
  std::cout << "* employee 2:" << std::endl;
  print(employee2); // invalid print
}

/**
 * Инициализация "по-умолчанию"
 */
struct Employee {
  char name[32] = "not defined";
  char surname[32] = "not defined";
  int age = -1;
  char office[32] = "not defined";
  enum {
    Undefined, Male, Female
  } sex = Undefined;
};

inline char const* str(decltype(Employee::Male) sex)
{
  return
    sex == Employee::Male ? "male" :
    sex == Employee::Female ? "female" : 
    "undefined";
}

/**
 * Передача немодифицируемой переменной по ссылке
 */
inline void print(Employee const& employee)
{
  std::cout
    << "name: " << employee.name << " " << employee.surname  << "\n"
    << "age: " << employee.age << "\n"
    << "office: " << employee.office << "\n"
    << "sex: " << str(employee.sex)  << std::endl;
}

EXAMPLE_FUNCTION(struct_default_initialization, 0)
{
  Employee employee1 {"Max", "Surov", 33, "11.3.21", Employee::Male};
  Employee employee2; // initialized with default values
  std::cout << "* employee 1:" << std::endl;
  print(employee1);
  std::cout << "* employee 2:" << std::endl;
  print(employee2);
}

/**
 * Динамический массив
 */
struct IntArray
{
  int sz = 0;
  int* data = nullptr;

  IntArray() = default;

  IntArray(int sz) // конструктор
  {
    std::cout << "IntArray(int)" << std::endl;
    assert(sz >= 0);
    this->sz = sz;
    this->data = new int[sz];
  }
  // IntArray(IntArray& s) // конструктор копий
  // {
  //     std::cout << "IntArray copy" << std::endl;
  //     sz = s.sz;
  //     data = new int[sz];
  //     memcpy(data, s.data, sizeof(int) * sz);
  // }
  IntArray(IntArray const& s) // конструктор копий
  {
    std::cout << "IntArray const copy" << std::endl;
    sz = s.sz;
    data = new int[sz];
    memcpy(data, s.data, sizeof(int) * sz);
  }
  // IntArray(IntArray const& s) // конструктор копий
  // {
  //     std::cout << "IntArray copy" << std::endl;
  //     sz = s.sz;
  //     data = new int[sz];
  //     memcpy(data, s.data, sizeof(int) * sz);
  // }
  IntArray(IntArray&& s) // конструктор перемещения
  {
    std::cout << "IntArray move" << std::endl;
    sz = s.sz;
    data = s.data;
    s.sz = 0;
    s.data = nullptr;
  }
  ~IntArray() // деструктор
  {
    std::cout << "~IntArray" << std::endl;
    if (this->data)
        delete[] this->data;
  }
};

void print(IntArray const& arr)
{
  std::cout << "[";
  for (int i = 0; i < arr.sz; ++ i)
    std::cout << arr.data[i] << (i == arr.sz - 1 ? "" : ", ");
  std::cout << "]" << std::endl;
}

EXAMPLE_FUNCTION(struct_constructor_and_distructor, 0)
{
  /**
   * Создаём два массива: из 10 элементов и пустой
   */
  IntArray arr(10);
  for (int i = 0; i < arr.sz; ++ i)
    arr.data[i] = i;
  print(arr);
  IntArray arr2;
  print(arr2);
}

int find_minimal(IntArray arr)
{
  if (arr.sz <= 0)
    return -1;
  int imin = 0;
  for (int i = 1; i < arr.sz; ++ i)
    imin = arr.data[i] < arr.data[imin] ? i : imin;
  return imin;
}

EXAMPLE_FUNCTION(struct_copy, 0)
{
  IntArray arr1(5);
  const IntArray arr3 = arr1; // создаём копию arr1
  IntArray arr4(arr3); // ещё одну
  find_minimal(arr1); // ещё одну
}

EXAMPLE_FUNCTION(this_pointer, 0)
{
  struct SomeStruct
  {
    SomeStruct()
    {
      static_assert(
        std::is_same<decltype(this), SomeStruct*>::value
      );
      std::cout << "this address: " << this << std::endl; // 0x7ffc20902f07
    }
  };

  SomeStruct a;
}

EXAMPLE_FUNCTION(pointer_to_struct_members, 0)
{
  struct A
  {
    int x;
    double y;
  };

  int A::* px = &A::x;
  double A::* py = &A::y;

  A a {1, 2.};

  assert(a.*px == a.x);
  assert(a.*py == a.y);
}

EXAMPLE_FUNCTION(const_struct_members, 0)
{
  struct A
  {
    const int c;
    int a;
    int g;

    A(int arg) : 
      c(arg + 5), a(10)
    {
      this->g = 3;
      a += g;
    }
  };

  A a(3);

  struct B
  {
    int x,y;
    B(int x, int y) : x(x), y(y) {}
  };
}


IntArray sequence(int n)
{
  IntArray a(n);
  for (int i = 0; i < n; ++ i)
    a.data[i] = i;
  return a;
}


EXAMPLE_FUNCTION(move_constructor, 0)
{
  IntArray arr = sequence(5);
  IntArray arr2(std::move(arr));
  assert(arr.data == nullptr);
  assert(arr2.data != nullptr);
}

uint32_t le2be(uint32_t arg)
{
  union uint8x4
  {
    uint32_t x;
    struct {
      uint8_t x1, x2, x3, x4;
    } bytes;
  };

  uint8x4 a {arg};
  uint8x4 b { .bytes = {a.bytes.x4, a.bytes.x3, a.bytes.x2, a.bytes.x1} };
  return b.x;
}

EXAMPLE_FUNCTION(union_example, 0)
{
  uint32_t a = le2be(0x12345678);
  assert(a == 0x78563412);


  union uint8x4
  {
    uint32_t x;
    struct {
      uint8_t x1, x2, x3, x4;
    } bytes;
  };

  uint8x4 ip { .bytes = {192, 168, 0, 1}}; // 0x0100a8c0
  printf("%08x", ip.x);
}


EXAMPLE_FUNCTION(struct_type_inside, 0)
{
  struct A
  {
    struct B
    {
      int x,y;
    };

    using charx4 = char[4];

    int z;
    B b;
    charx4 c;
  };

  A a;
  A::B b;
  A::charx4 c;
}

namespace struct_static_member
{
  struct A
  {
    inline static int x = 4; // declaration+definition
  };

  struct Z
  {
    // integral types
    static const int z = 5;
    // literal types
    static constexpr char s[] = "abc";
    static constexpr std::complex c {1,2};
  };

  struct W
  {
    const int z = 5;
  };

  EXAMPLE_FUNCTION(struct_static_member, 0)
  {
    A a, b;

    // assert(a.x == 4);
    assert(&a.x == &b.x);
    a.x = 1;
    b.x ++;
    assert(a.x == 2);

    assert(sizeof(Z) == 1);
    assert(sizeof(W) == 4);
  }
}

namespace named_constructor
{
  EXAMPLE_FUNCTION(named_constructor, 0)
  {
    class Temperature
    {
    public:
      static Temperature Fahrenheit (double f) { return Temperature((f + 459.67) / 1.8); }
      static Temperature Celsius (double c) { return Temperature(c + 273.15); }
      static Temperature Kelvin (double k) { return Temperature(k); }
      double value() const { return _temp; }
    private:
      Temperature(double temp) : _temp(temp) {}
      double _temp;
    };

    auto t = Temperature::Fahrenheit(100);
    std::cout << t.value() << std::endl;
  }
}

EXAMPLE_FUNCTION(decompose, 0)
{
  int arr[3] = {8,7,9};
  auto const [a1,a2,a3] = arr;
  assert(a1 == 8 && a2 == 7 && a3 == 9);

  struct Vec {int x; float y;};

  Vec a{5, 3.f};
  auto& [rx,ry] = a;
  assert(rx == 5 && ry == 3.f);
  assert(&rx == &a.x);
}
