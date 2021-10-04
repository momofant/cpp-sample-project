#include <iostream>
#include <assert.h>
#include "examples.h"

static void example31()
{
    /**
     * указатель: адрес локальной переменной (идут друг за другом)
     */
    int a = 0;
    int b = 1;
    auto pa = &a;
    auto pb = &b;
    std::cout << "address of a = " << pa << ";" << std::endl
        << "address of b = " << pb << ";" << std::endl;
}

EXAMPLE_FUNCTION(pointer_type, 0)
{
    /**
     * тип "указатель"
     */
    float x;
    auto px = &x;
    static_assert(std::is_same<decltype(px), float*>::value);
}

EXAMPLE_FUNCTION(access_bytes_of_integer_by_pointer, 0)
{
    /**
     * little-endian, big-endian
     * little-endian processor: 0x44 0x33 0x22 0x11
     * big-endian: 0x11 0x22 0x33 0x44
     */
    uint32_t x = 0x11223344;
    uint32_t* p1 = &x;
    uint8_t* p2 = reinterpret_cast<uint8_t*>(p1);

    printf("0x%08x is composed of bytes [0x%02x, 0x%02x, 0x%02x, 0x%02x]\n",
        x,
        *p2,
        *(p2 + 1),
        *(p2 + 2),
        *(p2 + 3)
    );
}

EXAMPLE_FUNCTION(bool_operator_for_pointers, 0)
{
    /**
     * преобразование указателя к типу bool
     */
    int x;
    int* px = &x;
    assert(bool(px));
    std::cout << "bool(&x) is not nullptr: " << bool(px) << std::endl;
}

EXAMPLE_FUNCTION(pointer_type_definition, 0)
{
    /**
     * в данном случае * относится не к типу, а к имени переменной
     */
    int *a, b;
    static_assert(std::is_same<decltype(a), int*>::value);
    static_assert(std::is_same<decltype(b), int>::value);

    std::cout
        << "pointer to integer type: " << typeid(a).name() << "\n"
        << "integer type: " << typeid(b).name() << std::endl;
}

EXAMPLE_FUNCTION(cast_pointer_to_intptr, 0)
{
    /**
     * указатель на начало и конец переменнйо в памяти
     * разница между указателями равна размеру типа
     */
    uint64_t x;
    intptr_t dif = intptr_t(&x + 1) - intptr_t(&x);
    assert(sizeof(uint64_t) == dif);
}

EXAMPLE_FUNCTION(pointers_cast, 0)
{
    /**
     * преобразование указателей: static_cast, reinterpret_cast
     *  преобразование к типу void* даёт произвольный доступ к памяти,
     *  в которой хранится переменная
     */
    uint32_t ui32;
    uint32_t* pui32 = &ui32;
    void* m = &ui32;
    uint8_t* pui8 = static_cast<uint8_t*>(m);

    // эквивалентно
    uint8_t* pui8_ = reinterpret_cast<uint8_t*>(&ui32);
    assert(pui8 == pui8_);
}

EXAMPLE_FUNCTION(references, 0)
{
    /**
     * тип: ссылка или скрытый указатель
     */
    int x = 0;
    int& rx = x;
    ++ rx;
    assert(x == 1 && rx == 1);
    ++ x;
    assert(x == 2 && rx == 2);
    assert(&x == &rx);

    // int& ry; // ошибка: ссылка должна быть сразу инициализирована
}
