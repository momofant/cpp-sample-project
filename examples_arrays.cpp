#include <iostream>
#include <string.h>
#include <assert.h>
#include "examples.h"

void example38()
{
    // способы оюъявления массивов
    int a[15];
    int b[3] = {1,8,4};
    int c[] {1,8,4};

    std::cout << sizeof(a) << std::endl;
    std::cout << sizeof(b) << std::endl;
    std::cout << sizeof(c) << std::endl;
}

void example39()
{
    // массив указателей
    int a1,a2,a3;
    int* a[3] {&a1, &a2, &a3};
    std::cout << typeid(a).name() << std::endl;

    float x, arr[2], *arrptrs[3];

    std::cout << typeid(x).name() << std::endl; // f
    std::cout << typeid(arr).name() << std::endl; // A2_f
    std::cout << typeid(arrptrs).name() << std::endl; // A3_Pf
}

// void example40()
// {
//     int n;
//     std::cin >> n;
//     float arr[n];

//     for (int i = 0; i < n; ++ i)
//         arr[i] = i;

//     float sum = 0.f;
//     for (int i = 0; i < n; ++ i)
//         sum += arr[i];

//     std::cout << (0 + n - 1) * n / 2 << " == " << sum << std::endl;
// }

void example42()
{
    uint16_t arr[5];

    // первый способ обращения к элементам массива
    int nelems = sizeof(arr) / sizeof(*arr);
    for (int i = 0; i < nelems; ++ i)
        arr[i] = 0;

    // второй способ обращения к элементам массива
    auto pstart = arr;
    auto pend = arr + nelems;
    for (auto p = pstart; p < pend; ++ p)
        *p = 0xFFFF;
}

void example43()
{
    using int_x3 = int[3];
    int_x3 arr1[5];
    int arr2[5][3];

    bool ok = std::is_same<
        decltype(arr1),
        decltype(arr2)
    >::value;

    std::cout << ok << std::endl;

    int d1 = sizeof(arr1) / sizeof(arr1[0]);
    int d2 = sizeof(arr1[0]) / sizeof(arr1[0][0]);

    for (int i1 = 0; i1 < d1; ++ i1)
        for (int i2 = 0; i2 < d2; ++ i2)
            arr1[i1][i2] = i1 + i2;
}

void example44()
{
    int arrsz = 23;
    int* arr;
    arr = new int[arrsz];
    for (int* p = arr; p < arr + arrsz; ++ p)
        *p = -1;
    delete [] arr;
}

void example45()
{
    int d1 = 3, d2 = 5;

    // allocate the array
    int** arr = new int*[d1];
    for (int row = 0; row < d1; ++ row)
        arr[row] = new int[d2];

    // access elements
    for (int i = 0; i < d1; ++ i)
        for (int j = 0; j < d2; ++ j)
            arr[i][j] = 0xff;

    // delete the array
    for (int row = 0; row < d1; ++ row)
        delete [] arr[row];

    delete [] arr;
}

void example46()
{
    int d1 = 3, d2 = 5;

    // allocate the array
    int* arr = new int[d1 * d2];

    // access elements
    for (int i = 0; i < d1; ++ i)
        for (int j = 0; j < d2; ++ j)
            arr[i*d2 + j] = 0xff;

    // delete the array
    delete [] arr;
}

void example47()
{
    int arr[][3] = {
        {1,2,3},
        {4,5,6}
    };
}

void example49()
{
    char s1[] = "abc";
    char s2[4] = {'a', 'b', 'c', '\0'};

    static_assert(
        std::is_same<decltype(s1),decltype(s2)>::value,
        "expect the char[4] and char[4]"
    );
}

void example50()
{
    char s_utf8[] = "привет!"; // 6*2 + 1 + 1 = 14
    std::cout << sizeof(s_utf8) << std::endl;

    char16_t s_utf16[] = u"привет!"; // 6*2 + 2 + 2 = 16
    std::cout << sizeof(s_utf16) << std::endl;

    char32_t s_utf32[] = U"привет!"; // 6*4 + 4 + 4 = 32
    std::cout << sizeof(s_utf32) << std::endl;
}

// void example51()
// {
//     char s1[] = "2 + 2";
//     char s2[] = " = 4";

//     char dst[strlen(s1) + strlen(s2) + 1];

//     strcpy(dst, "");
//     strcat(dst, s1);
//     strcat(dst, s2);

//     std::cout << dst << std::endl;
// }

void example59()
{
    char const s[] = R"(\t\n)"; 
    // or char const s[] = u8R"(\t\n)"; 
    std::cout << s << std::endl; // \t\n
}

EXAMPLE_FUNCTION(multipointers, 0)
{
    int x = 3;
    int* px = &x;
    int** ppx = &px;
    assert(**ppx == 3);
}

EXAMPLE_FUNCTION(static_arrays, 0)
{
  char s1[10]; // объявления массива из 10 символов
  char s2[10] = "abc"; // объявление массива и его инициализация
  const char s3[10] = "abc"; // в статической константной памяти будет выделен массив из 10 элементов 
    // и первые 3 будут проинициализированы abc, остальные - нулями
  char const* s4 = "abc"; // указатель на константную строку
  static const char* s5 = "abc"; // указатель на статическую строку
  static const char s6[] = "abc";

  // const_cast<char*>(s3)[1] = 'd'; // ок
  // const_cast<char*>(s4)[1] = 'd'; // ошибка
  // const_cast<char*>(s5)[1] = 'd'; // ошибка
  // const_cast<char*>(s6)[1] = 'd'; // ошибка

  std::cout << "abc\bdef\fxxx\vzzzz" << std::endl;
}

void allocate_memory(void** p, int sz)
{
  *p = new char[sz];
}

void free_memory(void** p)
{
  delete[] static_cast<char*>(*p);
  *p = nullptr;
}

EXAMPLE_FUNCTION(double_pointer, 0)
{
  void *p;
  allocate_memory(&p, 10);
  free_memory(&p);
}
