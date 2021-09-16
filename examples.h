#pragma once
#include <functional>
#include <map>


typedef void (*ExampleFunction)();

struct AddExample
{
	static inline std::map<char const*, ExampleFunction> examples;

	AddExample(char const* s, ExampleFunction e)
	{
		examples.emplace(s, e);
	}
};

#define CONCAT(a, b) CONCAT_(a, b)
#define CONCAT_(a, b) a ## b


#define ADD_EXAMPLE(fname) \
	static AddExample CONCAT(__, fname) (#fname, fname);

#define ADD_EXAMPLE_0(fname)
#define ADD_EXAMPLE_1(fname) ADD_EXAMPLE(fname)

#define EXAMPLE_FUNCTION(NAME, ENABLED) \
	static void NAME(); \
	CONCAT(ADD_EXAMPLE_, ENABLED)(NAME) \
	static void NAME()
