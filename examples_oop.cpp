#include <iostream>
#include <any>
#include <fstream>
#include <cstring>
#include <sstream>
#include <assert.h>
#include <ctime>
#include <complex>
#include <chrono>
#include <exception>
#include <Eigen/Dense>
#include "examples.h"
#include "anothefile.h"


struct IntArray
{
  int sz = 0;
  int* data = nullptr;

  void resize(int newsz)
  {
    assert(newsz >= 0);

    if (newsz == sz)
      return;

    if (!data)
    {
      sz = newsz;
      data = new int[newsz];
      return;
    }

    int* data = new int[newsz];
    if (newsz > sz)
    {
      std::memcpy(data, data, sizeof(int)*sz);
      std::memset(data + sz, 0, sizeof(int)*(newsz - sz));
    }
    else
    {
      std::memcpy(data, data, sizeof(int)*newsz);
    }

    delete[] data;
    data = data;
    sz = newsz;
  }

  int& at(int i)
  {
    return this->data[i];
  }
};

class X
{
private:
  int a;
  void f() { -- b; -- a; } // ok
public:
  int b;
  void g() { ++ b; ++ a; } // ok
  int get_a() const { return a; } // ok
  // int const& get_a() const { return a; } // ok
  // void u() const { ++ b; } // ошибка
  void d() const
  {
    static_assert(std::is_same<decltype(this), X const*>::value);
  }
};

EXAMPLE_FUNCTION(class_methods, 0)
{
  X x;
  // x.f(); // ошибка
  // x.a; // ошибка
  x.g(); // ок
  x.b; // ок
}

class Z
{
private:
  static int _x;
  int _v;

public:
  static void f()
  {
    ++ _x; // ok
    // -- _v; // error
  }
};

namespace inheritance_example {

  class A
  {
  private:
    int _a;
  protected:
    int _b;
  public:
    int c;
  };

  class B : public A
  {
  private:
    int _d;
  public:
    int e;

    void f()
    {
      // this->_a; // ошибка
      this->_b;
      this->c;
      this->_d;
      this->e;
    }
  };

  EXAMPLE_FUNCTION(inheritance, 0)
  {
    B x;

    // x._a; // ошибка
    // x._b; // ошибка
    x.c;
    // x._d; // ошибка
    x.e;
  }

}

struct Date
{
  int year, month, day;
};

Date now()
{
  std::time_t t = std::time(nullptr);
  std::tm *plt = std::localtime(&t);
  return {
    plt->tm_year + 1900,
    plt->tm_mon + 1,
    plt->tm_mday
  };
}

inline std::string str(Date const& d)
{
  char buf[64];
  sprintf(buf, "%02d/%02d/%04d", d.day, d.month, d.year);
  return buf;
}

constexpr int years_passed(Date const& from, Date const& to)
{
  bool b = (to.month > from.month) || 
    (to.month == from.month && to.day >= from.day);
  return to.year - from.year - 1 + b;
}

constexpr int monthes_passed(Date const& from, Date const& to)
{
  bool b = to.day >= from.day;
  return (to.year - from.year) * 12 + (to.month - from.month) - 1 + b;
}

static_assert(years_passed({2000, 1, 1}, {2000, 1, 1}) == 0);
static_assert(years_passed({2000, 1, 1}, {2001, 1, 1}) == 1);
static_assert(years_passed({2000, 1, 1}, {2001, 10, 10}) == 1);
static_assert(years_passed({2000, 1, 1}, {2001, 12, 31}) == 1);
static_assert(years_passed({2000, 12, 31}, {2001, 12, 30}) == 0);
static_assert(years_passed({2000, 12, 31}, {2001, 11, 30}) == 0);
static_assert(years_passed({2000, 6, 10}, {2001, 6, 10}) == 1);
static_assert(years_passed({2000, 6, 10}, {2001, 5, 10}) == 0);
static_assert(years_passed({2000, 6, 10}, {2001, 6, 9}) == 0);
static_assert(years_passed({2000, 6, 10}, {2001, 6, 11}) == 1);

static_assert(monthes_passed({2000, 1, 1}, {2000, 1, 1}) == 0);
static_assert(monthes_passed({2000, 1, 1}, {2001, 1, 1}) == 12);
static_assert(monthes_passed({2000, 1, 1}, {2001, 10, 10}) == 21);
static_assert(monthes_passed({2000, 1, 1}, {2001, 12, 31}) == 23);
static_assert(monthes_passed({2000, 12, 31}, {2001, 12, 30}) == 11);
static_assert(monthes_passed({2000, 12, 31}, {2001, 11, 30}) == 10);
static_assert(monthes_passed({2000, 6, 10}, {2001, 6, 10}) == 12);
static_assert(monthes_passed({2000, 6, 10}, {2001, 5, 10}) == 11);
static_assert(monthes_passed({2000, 6, 10}, {2001, 6, 9}) == 11);
static_assert(monthes_passed({2000, 6, 10}, {2001, 6, 11}) == 12);

class Person
{
protected:
  char _name[30];
  Date _birth;

public:
  Person(
    char const* name,
    Date const& birth
  )
  {
    strncpy(_name, name, sizeof(_name));
    _birth = birth;
  }
  inline int age() const
  {
    return years_passed(_birth, now());
  }
  inline char const* name() const
  {
    return _name;
  }
};

class Employee : public Person
{
protected:
  Date _employment;

public:
  Employee(
    char const* name,
    Date const& birth,
    Date const& employment
  ) :
    Person(name, birth),
    _employment(employment)
  {
  }
  inline int works_monthes() const
  {
    return monthes_passed(_employment, now());
  }
  inline int employed_at() const
  {
    return years_passed(_birth, _employment);
  }
};

inline void print(Employee const& e)
{
  int m = e.works_monthes();
  std::cout << "Name: " << e.name() << "\n"
    "Age: " << e.age() << "y" "\n"
    "Works: " << m / 12 << "y " << m % 12 << "m" << std::endl;
}

EXAMPLE_FUNCTION(employee, 0)
{
  Employee empl("Max", {1988, 7, 6}, {2020, 5, 4});
  print(empl);
}

namespace same_name_methods_access_example 
{
  class A
  {
  public: char f() { return 'A'; }
  };

  class B : public A
  {
  public: char f() { return 'B'; }
  };

  EXAMPLE_FUNCTION(same_name_methods_access, 0)
  {
    B b;
    A& a = b;

    assert(b.f() == 'B');
    assert(a.f() == 'A');
    assert(b.B::f() == 'B');
    assert(b.A::f() == 'A');
  }
}

namespace multilevel_inheritance
{
  class A {};
  class B : public A {};
  class C : public B {};
}

namespace access_base_class
{
  class A
  {
  public: int x;
  };

  class B : public A
  {
  public: int y;
  };

  EXAMPLE_FUNCTION(access_base_class, 0)
  {
    B b {1,2};
    assert(static_cast<A>(b).x == b.x);
    A* pa = &b;
    pa->x;
    B* pb = static_cast<B*>(pa);
    pb->y;
    A& ra = b;
    assert(ra.x == b.x);
    assert(&ra.x == &b.x);
  }
}

namespace multiple_inheritance
{
  struct B1
  {
    int b1;
  };

  struct B2
  {
    int b2;
  };

  struct C : B1, B2
  {
    int c;
  };

  EXAMPLE_FUNCTION(multiple_inheritance, 0)
  {
    C x;

    B1* pb1 = &x;
    B2* pb2 = &x;

    static_cast<C*>(pb1);
    static_cast<C*>(pb2);

    x.b1;
    x.b2;
    x.c;
  }
}


namespace diamond_inheritance
{
  struct A
  {
    char a[4];
    A(char const* s) { std::strncpy(a, s, sizeof(a)); }
  };
  struct B1 : A
  {
    char b1[4];
    B1() : A("a1"), b1{"b1"} {}
  };
  struct B2 : A
  {
    char b2[4];
    B2() : A("a2"), b2{"b2"} {}
  };
  struct C : B1, B2
  {
    char c[4];
    C() : B1(), B2(), c{"c"} {}
  };

  EXAMPLE_FUNCTION(diamond_inheritance, 0)
  {
    C c;
    // c.a; // ошибка
    assert(!strcmp(c.B1::a, "a1"));
    assert(!strcmp(c.B2::a, "a2"));
    assert(!strcmp(c.b1, "b1"));
    assert(!strcmp(c.b2, "b2"));
    assert(!strcmp(c.c, "c"));
  }
}

void printmem(void const* m, int sz)
{
  uint8_t const* b = static_cast<uint8_t const*>(m);

  for (int i = 0; i < sz; ++ i)
  {
    printf("%02x|", int(b[i]));
  }
}

namespace diamond_inheritance_2
{
  struct A
  {
    char a[4];
    A(char const* s) { std::strncpy(a, s, sizeof(a)); }
  };
  struct B1 : virtual A
  {
    char b1[4];
    B1() : A("a1"), b1{"b1"} {}
  };
  struct B2 : virtual A
  {
    char b2[4];
    B2() : A("a2"), b2{"b2"} {}
  };
  struct C : B1, B2
  {
    char c[4];
    C() : A("a"), B1(), B2(), c{"c"} {}
  };

  void print(B2 const& b)
  {
    std::cout << &b.a << " " << &b.b2 << std::endl;
  }

  EXAMPLE_FUNCTION(diamond_inheritance_2, 0)
  {
    C c;
    c.a;
    assert(!strcmp(c.B1::a, "a"));
    assert(!strcmp(c.B2::a, "a"));
    assert(!strcmp(c.b1, "b1"));
    assert(!strcmp(c.b2, "b2"));
    assert(!strcmp(c.c, "c"));

    B1 b1;
    assert(!strcmp(b1.a, "a1"));

    B2 b2;
    assert(!strcmp(b2.a, "a2"));

    memset(b1.a, 'A', 4);
    memset(b1.b1, 'B', 4);

    memset(b2.a, 'A', 4);
    memset(b2.b2, 'B', 4);

    memset(c.a, 'A', 4);
    memset(c.b1, 'B', 4);
    memset(c.b2, 'C', 4);
    memset(c.c, 'D', 4);

    // printmem(&data, sizeof(&data));
    // printf("\n");
    printmem(reinterpret_cast<void*>(&b1), sizeof(b1));
    printf("\n");
    printmem(reinterpret_cast<void*>(&b2), sizeof(b2));
    printf("\n");
    printmem(reinterpret_cast<void*>(&c), sizeof(c));
    printf("\n");

    // B1* pcb1 = &c;
    // B2* pcb2 = &c;

    // std::cout << sizeof(B1) << std::endl;

    // print(*pcb2);
    // print(b2);
   

    // std::cout << &pb1->a << std::endl;
    // std::cout << &pb2->a << std::endl;
  }
}

namespace static_polymorphism
{
  class Animal {
  public:
    void voice() { std::cout << "not implemented" << std::endl; }
  };

  class Cat : public Animal {
  public:
    void voice() { std::cout << "meou meou" << std::endl; }
  };

  class Dog : public Animal {
  public:
    void voice() { std::cout << "wow wow\n"; }
  };

  EXAMPLE_FUNCTION(static_polymorphism, 0)
  {
    Cat cat;
    Animal* pcat = &cat;
    Dog dog;
    Animal* pdog = &dog;

    Animal* zoo[2] = {
      pcat, pdog
    };

    zoo[0]->voice();
    zoo[1]->voice();
  }
}

namespace runtime_polymorphism
{
  class Animal {
  public:
    virtual void voice() { std::cout << "not implemented" << std::endl; }
  };

  class Cat : public Animal {
  public:
    void voice() override { std::cout << "meow meow" << std::endl; }
  };

  class Dog : public Animal {
  public:
    void voice() override { std::cout << "wow wow\n"; }
  };

  EXAMPLE_FUNCTION(runtime_polymorphism, 0)
  {
    Cat cat;
    Dog dog;
    Animal* zoo[2] = {
      &cat, &dog
    };
    zoo[0]->voice();
    zoo[1]->voice();

    printmem(&cat, sizeof(cat));
    printf("\n");
    printmem(&dog, sizeof(dog));
    printf("\n");
  }

  EXAMPLE_FUNCTION(virtual_destructor, 0)
  {
    Animal* animal = new Cat;
    // ...
    delete animal;
  }
}

namespace base_class_destructor_call
{
  struct A {
    A() {
      std::cout << "A" << std::endl;
    }
    ~A() {
      std::cout << "~A" << std::endl;
    }
  };

  struct B : A {
    B() {
      std::cout << "B" << std::endl;
    }
    ~B() {
      std::cout << "~B" << std::endl;
    }
  };

  EXAMPLE_FUNCTION(base_class_destructor_call, 0)
  {
    B b; // A B ~B ~A
  }
}

namespace base_class_virtual_destructor_call
{
  struct A {
    A() {
      std::cout << "A" << std::endl;
    }
    virtual ~A() {
      std::cout << "~A" << std::endl;
    }
  };

  struct B : A {
    B() {
      std::cout << "B" << std::endl;
    }
    ~B() {
      std::cout << "~B" << std::endl;
    }
  };

  EXAMPLE_FUNCTION(base_class_virtual_destructor_call, 0)
  {
    B b; // A B ~B ~A
  }
}

namespace rtti
{

  struct A {
    A() {
      std::cout << "A" << std::endl;
    }
    virtual ~A() {
      std::cout << "~A" << std::endl;
    }
  };

  struct B : A {
    B() {
      std::cout << "B" << std::endl;
    }
    ~B() {
      std::cout << "~B" << std::endl;
    }
  };

  EXAMPLE_FUNCTION(rtti, 0)
  {
    B b;
    A* pb = &b;
    A a;
    A* pa = &a;
    std::cout << typeid(*pa).name() << std::endl; // 1A
    std::cout << typeid(*pb).name() << std::endl; // 1B
  }
}

void write(std::ostream& s, std::string const& msg)
{
  s.write(msg.c_str(), msg.size()); // ostream::write
  s.flush(); // ostream::flush
}

EXAMPLE_FUNCTION(fstream_demo, 0)
{
  try
  {
    std::fstream f;
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f.open("/tmp/1.txt", std::ios_base::out);
    write(f, "hello!\n");
    write(std::cout, "hello!\n");
  }
  catch (std::ios_base::failure const& e)
  {
    std::cerr << "file error: " << e.what() << std::endl;
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

EXAMPLE_FUNCTION(string_demo, 0)
{
  using namespace std::literals;

  std::string c = "c";
  auto s = "c"s; // literal converting c-string into std::string

}

EXAMPLE_FUNCTION(delete_multiple_inheritance, 0)
{
  struct B1 {
    int b1;
  };
  struct B2 {
    int b2;
    virtual ~B2() {}
  };
  struct C : B1, B2 {
    int c;
  };

  B2* p = new C;
  delete p; // ошибка!
}

EXAMPLE_FUNCTION(stdexception, 0)
{
  try
  {
    /* code */
  }
  catch (const std::bad_alloc& e)
  {
    // too much memory requested?
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

namespace friend_classes_and_functions
{
  class A {
    int x;
    friend int f(A const& a) {
      return a.x;
    }
  public:
    A(int x) : x(x) {}
  };

  class B {
    int z;
    friend int g(B* b);
  public:
    B(int z) : z(z) {}
  };

  int g(B* b) { return b->z; }

  class X {};
  class Z {
    friend X; // already declared
    friend class Y; // forward declaration
    class W {};
  };
  class Y : public Z::W {};

  EXAMPLE_FUNCTION(friend_classes_and_functions, 0)
  {
    A a(5);
    assert(f(a) == 5);
    B b(6);
    assert(g(&b) == 6);
  }
}

namespace interface
{
  class ISerializable
  {
  public:
    virtual ~ISerializable() {}
    virtual void save(std::ostream& s) const = 0;
    virtual void load(std::istream& s) = 0;
  };

  class Employee : public ISerializable
  {
  private:
    std::string _name;
    Date _birth;

  public:
    Employee(std::string const& name, Date const& birth) : _name(name), _birth(birth) {}
    ~Employee() {}
    void save(std::ostream& s) const
    {
      s << _name << " " << _birth.year << " " << _birth.month << " " << _birth.day << " ";
    }

    virtual void load(std::istream& s)
    {
      s >> _name >> _birth.year >> _birth.month >> _birth.day;
    }
  };

  EXAMPLE_FUNCTION(interface, 0)
  {
    Employee emp { "max", {1988, 7, 6}};
    std::stringstream ss;
    emp.save(ss);
    ss.seekg(0);
    emp.load(ss);
    std::cout << ss.str() << std::endl;
  }
}

namespace observer_pattern
{
  class IObserver {
  public:
    virtual ~IObserver() {}
    virtual void update() = 0;
  };

  class Subject {
    static const int _max_observers = 10;
    int _nobservers = 0;
    IObserver* _observers[_max_observers];
  public:
    void attach(IObserver* observer) {
      if (_nobservers == _max_observers - 1)
        throw std::runtime_error("too many observers");
      _observers[_nobservers] = observer;
      ++ _nobservers;
    }
    void detach(IObserver* observer) {
      // todo
    }
    void notifyall() {
      for (int i = 0; i < _nobservers; ++ i)
        _observers[i]->update();
    }
  };

  class ClockTimer : public Subject {
  public:
    void on_event(){
      notifyall();
    }
  };

  class WriteStdout : public IObserver {
  public:
    void update() override {
      std::cout << "updated" << std::endl;
    }
  };

  class RedrawClock : public IObserver {
  public:
    void update() override {
      // todo
    }
  };

  EXAMPLE_FUNCTION(f, 0)
  {
    ClockTimer clock;
    WriteStdout write;
    RedrawClock redraw;

    clock.attach(&write);
    clock.attach(&redraw);
    clock.on_event();
  }
}

namespace adapter_pattern
{
  class Interface {
  public:
    virtual ~Interface() {}
    virtual void dosomething() = 0;
    // ...
  };

  class Implementation {
  public:
    void f() {}
    int g(int) { return 5; }
  };

  class Adapter : public Interface {
    Implementation* _impl;
    void dosomething() override
    {
      _impl->f();
      _impl->g(1);
    }
  };
}


namespace prototype_pattern
{
  class IClonable {
  public:
    virtual ~IClonable() {}
    virtual IClonable* clone() = 0; // make a copy
  };

  class Circle : public IClonable {
    Circle* clone() override {
      return nullptr;
    }
  };
}

namespace decorator_pattern
{
  class Component {
  public:
    virtual ~Component() {}
    virtual void dosomething() = 0;
  }; 

  class Decorator : public Component {
  protected:
    Component* _component;
  public:
    Decorator(Component* component) : _component(component) {}
    void dosomething() override {
      _component->dosomething();
    }
  };

  class ConcreteDecoratorA : public Decorator {
  public:
    ConcreteDecoratorA(Component* component) : Decorator(component) {}
    void dosomething() override {
      Decorator::dosomething();
      // do something else here
    }
  };
}

namespace dynamic_type_cast
{
  class A {
  public:
    virtual ~A() {}
  };
  class B : public A {};
  class C : public B {};

  EXAMPLE_FUNCTION(dynamic_type_cast, 0)
  {
    B b;
    A* p = &b;
    assert(dynamic_cast<B*>(p) != nullptr);
    assert(dynamic_cast<C*>(p) == nullptr);
    dynamic_cast<B&>(*p);
    static_cast<C&>(b);

    if (auto pb = dynamic_cast<B*>(p)) {}

    try
    {
      // dynamic_cast<C&>(b); // throws std::bad_cast
    }
    catch(const std::bad_cast& e)
    {
      std::cerr << e.what() << '\n';
    }
    catch(const std::exception& e)
    {
      std::cerr << typeid(e).name() << '\n';
    }
    catch(...)
    {
      std::cerr << "undef exception" << '\n';
    }
  }
}

namespace covariant_return_types
{
  struct A { };
  struct B : A { };

  class X
  {
  private:
    A a;
  public:
    virtual A const& get() const { return a; }
  };

  class Y : public X
  {
  private:
    B b;
  public:
    B const& get() const override { return b; }
  };

  EXAMPLE_FUNCTION(covariant_return_types, 0)
  {
    Y y;
    y.get();
  }
}

namespace singleton
{
  class Logger
  {
  private:
    std::ofstream _out;

  public:
    Logger() {
      _out.exceptions(std::ofstream::badbit);
      _out.open("/tmp/log.txt");
    }
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    void write(std::string const& ) {}

    static Logger& instance()
    {
      static Logger logger;
      return logger;
    }
  };
}

namespace operator_overloading
{
  struct Vec3
  {
    double x,y,z;
    inline Vec3 operator + (Vec3 const& b) {
      return {x + b.x, y + b.y, z + b.z};
    }
    inline operator bool() const {
      return x != 0. || y != 0. || z != 0.;
    } 
  };

  // global operators
  inline double operator * (Vec3 const& a, Vec3 const& b) {
    // dot product
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }
  inline Vec3 operator ^ (Vec3 const& a, Vec3 const& b) {
    // cross product
    return {
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x
    };
  }
  inline std::ostream& operator << (std::ostream& s, Vec3 const& v)
  {
    s << "(" << v.x << "," << v.y << "," << v.z << ")";
    return s;
  }

  EXAMPLE_FUNCTION(operator_overloading, 0)
  {
    Vec3 a {1,2,3};
    Vec3 b {4,3,6};
    Vec3 c {3,-6,1};
    auto d = (a ^ b) + c;
    std::cout << a << " x " << b << " + " << c << " = " << d << std::endl;
  }

  class Array
  {
    float* _data;
    int _sz;
  public:
    Array(int sz) : _sz(sz), _data(new float[sz]) {}
    inline float& operator [](int i) {
      assert(i >= 0 && i < _sz);
      return _data[i];
    }
    inline float const& operator [](int i) const {
      assert(i >= 0 && i < _sz);
      return _data[i];
    }
  };

  EXAMPLE_FUNCTION(operator_overloading_2, 0)
  {
    Array arr(10);
    for (int i = 0; i < 10; ++ i)
      arr[i] = i*i;
    assert(arr[4] == 16);
  }

  struct A
  {
    inline void operator ++ ()
    {
      std::cout << "prefix" << std::endl;
    }

    inline void operator ++ (int)
    {
      std::cout << "postfix" << std::endl;
    }
  };

  // alternative
  // inline void operator ++ (A&, int) {...}

  EXAMPLE_FUNCTION(increment_overloading, 0)
  {
    A a;
    a++;
    ++a;
  }

  EXAMPLE_FUNCTION(types_convertion, 0)
  {
    Vec3 v {0., 0., 0.};
    assert(!v);
  }

  struct complex
  {
    double x, y;

    explicit complex(double x, double y=0.) : x(x), y(y) {}

    explicit inline operator double() const {
      assert(y == 0.);
      return x;
    }
    inline complex operator ~() const {
      return complex {x, -y};
    }
    inline complex operator * (complex const& z) const {
      return complex {x*z.x - y*z.y, x*z.y + y*z.x};
    }
    inline complex operator + (complex const& z) const {
      return complex {x+z.x, y + z.y};
    }
    inline bool operator == (complex const& z) const {
      return x == z.x && y == z.y;
    }
    inline complex& operator = (double x) {
      complex::x = x;
      y = 0.;
      return *this;
    }
  };

  EXAMPLE_FUNCTION(complex_operator_overloading, 0)
  {
    complex z {3, -4};
    double zsq(z * ~z);
    assert(zsq == 25);
    complex a {1, 2};
    complex b {2, -6};
    auto c = a + b;
    assert(a + b == z);

    c = a = 5.;
    assert(a == c);
  }

}

namespace operator_arrow_overloading
{
  struct C {
    int m;
  };
  struct B {
    C c;
    inline C const* operator -> () const {
      std::cout << "B->" << std::endl;
      return &c;
    }
  };
  struct A {
    inline B operator -> () const {
      std::cout << "A->" << std::endl;
      return B();
    }
  };
  EXAMPLE_FUNCTION(operator_arrow_overloading, 0)
  {
    A a;
    a->m; // A-> B->
  }

}

namespace explicit_example 
{
  class X {};

  struct S {
    S(X const* s) {}
  };

  EXAMPLE_FUNCTION(explicit_example, 0)
  {
    S s = 0; 
  }

  EXAMPLE_FUNCTION(not_operator, 0)
  {
    struct A {
      virtual int operator !() {
        return 5;
      }
    };
    struct B : A {
      int operator !() override {
        return 6;
      }
    };

    B b;
    A& a = b;
    std::cout << (!a) << std::endl;

  }
}

namespace efficient_strings
{
  class String {
    char* _data;
    int _reserved;
    int _sz;
    template <int N>
    friend struct Joined;
  public:
    String() : _data(nullptr), _sz(0), _reserved(0) {}
    explicit String(const char* s)
    {
      _sz = strlen(s);
      _reserved = _sz + 1;
      _data = new char[_reserved];
      memcpy(_data, s, _sz + 1);
    }
    explicit String(const char* s, int n)
    {
      _sz = n;
      _reserved = _sz + 1;
      _data = new char[_reserved];
      memcpy(_data, s, _sz);
      _data[_sz] = '\0';
    }
    int len() const {
      return _sz;
    }
  };

  template <int N>
  struct Joined {
    String const* strings[N];

    int total_len() const {
      int n = 0;
      for (int i = 0; i < N; ++ i) {
        n += strings[i]->len();
      }
      return n;
    }
    explicit inline operator String () const {
      int n = total_len();
      String result;
      result._reserved = n+1;
      result._sz = n;
      result._data = new char[result._reserved];
      char *p = result._data;
      for (int i = 0; i < N; ++ i) {
        memccpy(p, strings[i]->_data, 1, strings[i]->_sz);
        p += strings[i]->_sz;
      }
      *p = '\0';
      return result;
    }
  };

  template <int N>
  Joined<N+1> operator + (Joined<N> const& j, String const& s) {
    Joined<N+1> d;
    for (int i = 0; i < N; ++ i)
      d.strings[i] = j.strings[i];
    d.strings[N] = &s;
    return d;
  }

  Joined<2> operator + (String const& s1, String const& s2) {
    Joined<2> d;
    d.strings[0] = &s1;
    d.strings[1] = &s2;
    return d;
  }

  inline String operator ""_str (char const* s, size_t n) {
    return String(s, int(n));
  }

  EXAMPLE_FUNCTION(join_strngs, 0)
  {
    String s("abc"_str + "def"_str + "ghi"_str);
  }
}

EXAMPLE_FUNCTION(eigen_example, 0)
{
  Eigen::Vector3d a, b;
  {
    a << 1,2,3;
    b << 2,3,4;
    auto c = a + b; // result is Matrix Expression
    a << 5,5,5;
    std::cout << c.transpose() << std::endl; // 7,8,9
  }

  {
    a << 1,2,3;
    b << 2,3,4;
    Eigen::Vector3d c = a + b; // result is Matrix Expression
    a << 5,5,5;
    std::cout << c.transpose() << std::endl; // 3,5,7
  }
}

EXAMPLE_FUNCTION(any_demo, 0)
{
  using namespace std::literals;

  struct S {
    S(S const&) { std::cout << "S(S const&)" << std::endl; }
    S(S&&) { std::cout << "S(S&&)" << std::endl; }
    S() { std::cout << "S()" << std::endl; }
    ~S() { std::cout << "~S()" << std::endl; }
  };

  std::any a = 1;
  long* p = std::any_cast<long>(&a);
  assert(p == nullptr);

  a = S();

  a = "abc";
  std::cout << "to c-string: " << std::any_cast<char const*>(a) << std::endl;

  a = "def"s;
  std::cout << "to std::string: " << std::any_cast<std::string>(a) << std::endl;

  a = 1.0;
  try {
    std::any_cast<int>(a);
  }
  catch(const std::bad_any_cast& e)
  {
    std::cerr << e.what() << '\n';
  }
}

namespace final_demo
{
  struct A  {
    virtual void f() /* final */ {}
  };

  struct B : A {
    void f() override {}
  };

  struct C /* final */ {};
  struct D : C {};
}

namespace virtual_initialization 
{
  struct A {
    A() { f(); }
    ~A() { f(); }
    virtual void f() { std::cout << "A::f" << std::endl; }
  };
  struct B : A {
    B() { }
    ~B() { }
    void f() override { std::cout << "B::f" << std::endl; }
  };

  EXAMPLE_FUNCTION(virtual_initialization, 0)
  {
    B b;
  }
}

EXAMPLE_FUNCTION(three_way_cmp, 1) {
  struct A{
    int x;

    inline auto operator <=> (A const& other) const {
      return x <=> other.x;
    }
  };

  A a {1}, b{2};

  assert(a < b);
  assert(!(a > b));
  assert(!(a >= b));
  assert(a <= b);
}
