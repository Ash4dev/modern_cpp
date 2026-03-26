/*
 * File: random_tricks.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-03-22
 * Description: Only restrict to C++20 (minimize overload)
 * NOTE: Use standard library features: highly optimized
 */

// g++ -g -fsanitize=address random_tricks.cpt // identify memory leaks

#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <algorithm> // std::fill
#include <iterator>  // std::begin(), std::end()
#include <numeric>   // std::iota

#include <functional>  // std::function
#include <type_traits> // is_const, decltype

// NOTE: function pointers
typedef int (*bad_fn_ptr_t)(int, int); // must stay true to signature always
using good_fn_ptr =
    std::function<int(int, int)>; // wrapper around any callable (type erasure)

// NOTE: function overloading: func(3, 4) -> ambiguous function call
auto func(int a, float) -> float; // no signature mangling -> no overloading (C)
auto func(int a, double)
    -> float; // auto filler, actual return type float (C++11)
auto func(float, float)
    -> float; // if auto as return, all must return same type (C++14)
// function params can also have type auto (like func template) C++20

// NOTE: paramerter passing (pass by value, address and reference)
void function(const int *arr, int size); // pointer decay; sizeof not useful (C)
void function(const std::array<int, 34 * 2 / 3> &arr); // std::array parsing C++
void function(std::vector<int> &arr);

int main() {
  // NOTE: array & std algorithm library

  // homogeneous, contiguous, compile-time, pointer decay (fn param): C
  std::array<int, 34 * 2 / 3> arr; // no decay in C++

  // memset sets first N bytes of S to C. Fills each byte with 1
  std::fill(arr.begin(), arr.end(), 1); // Fills each element

  // fill in linear range with value++ (avoid manual for loop)
  std::iota(arr.begin(), std::end(arr), 0);

  // range-based for loop
  for (int element : arr) {
    break;
  }

  // NOTE: sizeof pitfalls: use for size with runtime containers, void pointers
  std::cout << sizeof(arr) << "\n"; // actual onject byte count, not size
  std::vector<int> vector;
  for (int i = 0; i < 1; ++i)
    vector.push_back(i);
  std::cout << "vector size: " << sizeof(vector) / sizeof(int) << std::endl;

  // NOTE: pointers (typecasting alters only size info) / also subtractable
  // nullptr derefernce / memory leaks / dangling pointer / double delete
  char x = 'a';
  std::cout << "address of x: " << &x << " " << (void *)&x
            << std::endl; // char * -> c string! prints a

  auto val = &arr;
  std::cout << "virtual addr inside process: " << val
            << " sizeof: " << sizeof(&x) << std::endl; // 8 bit machine

  // NOTE: mini-metaprogramming (const check)
  const int alpha{4};
  std::cout << "good inspection tool for constness: "
            << std::is_const<decltype(alpha)>::value << std::endl;
  return EXIT_SUCCESS;
}
