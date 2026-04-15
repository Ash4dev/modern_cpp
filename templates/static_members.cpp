/*
 * File: static_members.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-04-12
 * Description: Static members in templates, Class Template Argument Deduction
 * NOTE: keep it working & simple
 */

#include <iostream>
#include <memory>
#include <numeric>
#include <vector>
template <typename T> class Container {
private:
  std::size_t N;
  std::unique_ptr<T[]> m_data;
  std::string_view m_nature;

public:
  static std::size_t MAX_SIZE;
  Container() = default;

  Container(std::size_t _N)
      : N{std::min(_N, MAX_SIZE)}, m_data{std::make_unique<T[]>(N)},
        m_nature("Random") {

    std::iota(m_data.get(), m_data.get() + N, 0);
  }

  void print_values() const {
    for (std::size_t idx{0}; idx < N; ++idx) {
      std::cout << m_data[idx] << " ";
    }
    std::cout << '\n';
  }
};

// NOTE: CTAD
// Option 2: tells the compiler to default to a specific type (Deduction Guide)
Container(std::size_t) -> Container<int>;

// initialize the static member variable
template <typename T> std::size_t Container<T>::MAX_SIZE{10};

int main() {
  std::cout << (Container<int>::MAX_SIZE == Container<float>::MAX_SIZE)
            << std::endl;
  std::cout << Container<float>::MAX_SIZE << std::endl;

  // CTAD determines type from arguments. Here, no arg has type T. CTAD fails
  // Option 1: extra arg for type inference / Option 2: Deduction Guide (Used)
  Container c(15);
  c.print_values();

  // CTAD: C++17
  std::vector v1{1, 2}; // uses CTAD to determine vector<int>

  // compiler is not able to decide on i / f / d -> expects to be same type
  std::vector v2{1.0, 2.0, 3.0}; // vector<double>
  return 0;
}
