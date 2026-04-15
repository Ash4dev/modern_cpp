/*
 * File: intro.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-04-10
 * Description: Generics aka template in CPP
 * NOTE: keep it working & simple
 */

#include <cmath>
#include <cstddef>
#include <iostream>

// template can only be defined in a namespace / class
namespace template_space {
// template variable
template <typename T = long double> constexpr T pi{3.1415926536897932385L};
constexpr double radius{3};
constexpr auto diameter = 2 * radius;

// template arguments are a must for template variable - no other way to infer
constexpr auto circumference = pi<> * diameter;
constexpr auto area = pi<double> * radius * radius;
} // namespace template_space

////////////////////////////////////////////////////////////////////////////
// NOTE: blueprint for the COMPILER to generate corr TYPE code: COMPILE TIME
// enforces DRY: square<int> ..... square<UDT> at cost of bigger binary
// cost reduction strategies: definite ordering / min # type params
template <typename T> T square(T side) { return side * side; }

////////////////////////////////////////////////////////////////////////////
// why not custom_arr_init(T side, size_t N)? N: compile const / each N define.
// it is NOT mandatory to always pass in a type
template <typename T, std::size_t N> void custom_arr_init() {
  [[maybe_unused]] T arr[N];
  arr[0] = T{1};
  for (std::size_t i{1}; i < N; ++i) {
    arr[i] = T{static_cast<T>(arr[i - 1]) * 3};
    std::cout << "i: " << arr[i] << "\t";
  }
  std::cout << "\n";
}

////////////////////////////////////////////////////////////////////////////
// template param type must be explicitly provided or is inferred from arguments
// type T is mandatorily inferred, whereas U falls back to dobule if not given
template <typename T, typename U = double>    // template type param list
constexpr U average(T const &a, T const &b) { // function param list
  // a, b MUST be of the same type T
  return (a + b) / U{2};
}

////////////////////////////////////////////////////////////////////////////
// NOTE: clang diagnostic message is just a warning: requires C++20 & more
// 'auto' not allowed in function prototype [auto_not_allowed]: auto deduces
auto average2(auto a, auto b) { return (a + b) / 2.0f; }

////////////////////////////////////////////////////////////////////////////
typedef struct {
  int a;
  float b;
} UDT;

template <typename T1> bool is_equal(T1 const &a, T1 const &b) {
  return (a == b);
}

// NOTE: template specializations
// full specialization: different specialization for few types
// partial specialization: specialize for subset of types

// NOTE: function specialization template (only support full)
template <> bool is_equal(double const &a, double const &b) = delete;

template <> bool is_equal<UDT>(UDT const &a, UDT const &b) {
  std::cout << "custom specialized function utilized\n";
  return (a.a == b.a) and (fabs(a.b - b.b) < 0.00001f);
}

int main() {

  std::cout << template_space::pi<float> << std::endl;
  std::cout << template_space::area << std::endl;

  ////////////////////////////////////////////////////////////////////////////

  std::cout << square(5.5f) << std::endl; // template argument decduction

  // general practice for explicit param type call: potential ambiguous call
  std::cout << square<double>(5.5f) << std::endl; // double explicit call

  ////////////////////////////////////////////////////////////////////////////

  custom_arr_init<float, 3>();
  std::cout << average(3.0f, 3.5f) << std::endl; // MUST be of same type

  ////////////////////////////////////////////////////////////////////////////

  std::cout << average2(3.0f, 3.5) << std::endl;

  ////////////////////////////////////////////////////////////////////////////

  UDT u1{3, 4.23f};
  UDT u2{3, 4.23f};
  std::cout << is_equal(u1, u2);
  return 0;
}
