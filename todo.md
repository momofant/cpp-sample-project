
```c++
std::vector<int> foo = {25,15,5,-5,-15};
std::vector<int> bar;
```

copy if
[filter containers](https://www.cppstories.com/2021/filter-cpp-containers/)
```c++
copy only positive numbers:
std::copy_if (foo.begin(), foo.end(), std::back_inserter(bar), [](int i){return i>=0;} );
```


std::for_each(std::execution::par, begin(vec), end(vec),

[integer sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence)

https://en.cppreference.com/w/cpp/algorithm/ranges/for_each


consteval

Copy elision

// constexpr

Attribute specifier sequence 
    https://en.cppreference.com/w/cpp/language/attributes

explicit int arr[];
