#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <exception>
#include "examples.h"


void readfile(char const* s, char*& data, int& sz)
{
  FILE* f = fopen(s, "r");
  if (!f)
    throw "couldn't open file";

  fseek(f, 0, SEEK_END);
  sz = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (sz < 0)
  {
    fclose(f);
    throw "invalid file size";
  }

  data = new char[sz];
  int n = fread(data, 1, sz, f);
  fclose(f);

  if (n != sz)
  {
    delete[] data;
    throw "couldn't read overall file";
  }
}

void disp_file(char const* path)
{
  char* p;
  int sz;
  readfile(path, p, sz);
  std::cout << "file content: " << p << std::endl;
  delete[] p;
}

EXAMPLE_FUNCTION(read_file, 0)
{
  try
  {
    disp_file("/tmp/1.txt");
  }
  catch (char const* s)
  {
    std::cerr << "failed: " << s << std::endl;
  }
  catch (int const* arr)
  {
    std::cout << arr[0] << " " << arr[1] << std::endl;
  }
  catch (...)
  {
    std::cerr << "failed: undefined" << std::endl;
  }
}

EXAMPLE_FUNCTION(simple_exception, 0)
{
  try
  {
    throw "zzz";
  }
  catch (int errcode)
  {
    std::cerr << "cought " << errcode << std::endl;
  }
  catch (...)
  {
    std::cerr << "failed: undefined" << std::endl;
  }
}

void g() { static const char desc[] = "some error in g()"; throw desc; }
void f() { g(); }

EXAMPLE_FUNCTION(nested_try_catch, 0)
{
  try
  {
    try
    {
      f();
    }
    catch(int errcode)
    {
      std::cerr << "failed: " << errcode << std::endl;
    }
  }
  catch(char const* errdesc)
  {
    std::cerr << "failed: " << errdesc << std::endl;
  }
}

struct FileIOException
{
  char what[20];

  FileIOException(char const* s)
  {
    strncpy(what, s, sizeof(what));
    std::cout << this << std::endl;
  }

  ~FileIOException()
  {
    std::cout << "~FileIOException " << this << std::endl;
  }

  FileIOException(FileIOException&& from)
  {
    strcpy(what, from.what);
    strcpy(from.what, "");

    std::cout << "FileIOException moved" << std::endl;
  }
};

inline void f2(int)
{
  // auto e = FileIOException {"can't open file"};
  // throw e;
  throw FileIOException {"can't open file"};
}

inline void f1(int)
{
  f2(1);
}

EXAMPLE_FUNCTION(typed_exception, 0)
{
  try
  {
    f1(0);
  }
  catch (FileIOException const& e)
  {
    std::cerr << &e << std::endl;
  }
}

EXAMPLE_FUNCTION(bad_alloc, 0)
{
  try
  {
    char* p = new char[100000000000];
    delete [] p;
  }
  catch (std::bad_alloc const& e)
  {
    std::cerr << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "undefined" << std::endl;
  }
}

EXAMPLE_FUNCTION(rethrow, 0)
{
  try
  {
    try
    {
      throw std::runtime_error("thrown 2 times");
    }
    catch (std::runtime_error const& e)
    {
      std::cout << "cought first: " << e.what() << std::endl;
      throw e;
    }
  }
  catch(std::exception const& e)
  {
    std::cout << "cought second: " << e.what() << std::endl;
  }
}

EXAMPLE_FUNCTION(stdexceptions, 0)
{
  try
  {
    throw std::runtime_error("some description");
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  
}

inline void f(int x)
try
{
  if (x < 0) throw 1;
  std::cerr << "x >= 0" << std::endl;
}
catch (...)
{
  std::cerr << "x < 0" << std::endl;
}

EXAMPLE_FUNCTION(function_try_block, 0)
{
  f(-5);
} 

EXAMPLE_FUNCTION(function_try_block_2, 0)
{

  struct A{
    A(int x) { throw x; };
  };

  struct S {
    A a;
    S(int x) try : a(x) {
      // todo
    }
    catch (...) {
      std::cout << "cought inside constructor " << std::endl;
    }
  };

  S s(1);
} 
