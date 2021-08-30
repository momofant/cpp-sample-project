cmake_minimum_required(VERSION 3.2)
project(democpp CXX)

add_executable(
	democpp
	main.cpp
	anothefile.cpp
	anothefile.h
)
target_compile_features(democpp PUBLIC cxx_std_17)
target_compile_definitions(democpp PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
