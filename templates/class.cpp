/*
 * File: class.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-04-11
 * Description:
 * NOTE: keep it working & simple
 */
#include <cstddef>
#include <iostream>
#include <memory>
#include <numeric>
#include <string_view>

#define BAD_RETURN_VALUE -1

struct Non_Copyable {
  Non_Copyable() = default;
  // NOTE: avoid object slicing and frankenobject respectively
  Non_Copyable(Non_Copyable const &other) = delete;
  Non_Copyable &operator=(Non_Copyable const &other) = delete;
};

template <typename T> class Container : Non_Copyable {
private:
  std::size_t N;
  std::unique_ptr<T[]> m_data;
  std::string_view m_nature;

public:
  Container() = default;
  // NOTE: initialize in the same order as declaration order
  // Base Class Constructors -> Member variables

  // make_unique_for_overwrite is C++20 feature
  Container(std::size_t _N)
      : Non_Copyable(), N{_N}, m_data{std::make_unique<T[]>(N)},
        m_nature("Random") {

    // NOTE: access ptr address in unique_ptr using get
    std::iota(m_data.get(), m_data.get() + N, 0);
  }
  ~Container() = default;

  Container(Container &&src)
      : Non_Copyable(), N{src.N}, m_data{std::move(src.m_data)},
        m_nature{src.m_nature} {

    // leave src at valid empty state
    src.N = 0;
    src.m_data = nullptr;
    src.m_nature = std::string_view{};
  }

  Container &operator=(Container &&src) {
    if (this == &src)
      return *this;

    // steal resources from source
    N = src.N;
    m_data = std::move(src.m_data);
    m_nature = src.m_nature; // string_view is non-owning (move = copy)

    // leave src at valid empty state
    src.N = 0;
    src.m_data = nullptr;
    src.m_nature = std::string_view{};

    return *this;
  }

  T const &get_value(std::size_t idx) const {
    if (idx >= N)
      throw std::out_of_range("Index out of range");
    return m_data[idx];
  }

  void set_value(std::size_t idx, T const &val) {
    if (idx >= N)
      return;
    m_data[idx] = val;
  }

  void print_values() const {
    for (std::size_t idx{0}; idx < N; ++idx) {
      std::cout << m_data[idx] << " ";
    }
    std::cout << '\n';
  }
};

int main() {
  Container<int> a(5);
  a.set_value(0, 100'00);
  a.print_values();
  return 0;
}
