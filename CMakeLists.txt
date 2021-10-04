cmake_minimum_required(VERSION 3.2)
project(democpp CXX)

add_executable(
	democpp
	main.cpp
	anothefile.cpp
	anothefile.h
	examples_arrays.cpp
	examples_branches.cpp
	examples_enums.cpp
	examples_functions.cpp
	examples_loops.cpp
	examples_namespaces.cpp
	examples_pointers.cpp
	examples_preprocessor.cpp
	examples_simple_datatypes.cpp
	examples_struct.cpp
	examples_literals.cpp
	examples_exceptions.cpp
	examples_oop.cpp
	examples_templates.cpp
	examples_functional.cpp
	examples_threads.cpp
	examples_containers.cpp
)

target_compile_features(democpp PUBLIC cxx_std_20)

target_compile_options(democpp PUBLIC 
	-Wno-unused-value 
	-ftemplate-depth=2048
	-fconstexpr-depth=100000
	-pedantic-errors 
)
target_compile_definitions(democpp PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_link_libraries(democpp pthread)
