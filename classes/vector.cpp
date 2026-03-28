/*
 * File: classes_2.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-03-28
 * Description:
 * NOTE: keep it working & simple
 */

#include <algorithm>
#include <iostream>
#include <vector>

#define VEC_MAX_SIZE 5000

class Array {
private:
  std::vector<int> data;

public:
  // vector follows RAII: no explicit destructor is needed
  Array() = default;
  Array(int size) {
    data.resize(size);
    int i{};
    std::generate(data.begin(), data.end(), [&i]() mutable { return i * i++; });
  }

  Array(const Array &other) {
    std::cout << "copy-constructor called" << "\n";

    // // obtain a new memory block & fill it appropriately
    // std::vector<int> new_data(other.data.size());
    // std::copy(other.data.begin(), other.data.end(), new_data.begin());
    //
    // // assign the new data
    // data = new_data;

    // vector already handles deep copy
    data = other.data;
  }

  Array &operator=(const Array &copy) {
    std::cout << "copy assignment operator called" << "\n";

    // prevent copy to itself
    if (this == &copy)
      return *this;

    // // obtain new memory & fill appropriately with copy's data
    // std::vector<int> new_data(copy.data.size());
    // std::copy(copy.data.begin(), copy.data.end(), new_data.begin());
    //
    // // delete old data
    // data.clear();
    // data.resize(copy.data.size());
    //
    // // assign the new data
    // data = new_data;

    // vector already handles deep copy
    data = copy.data;
    return *this;
  }

  // NOTE: if copies are NOT desired, make private / delete them (modern)
  // copy-constructor: avoid creation of new objects from other such objects
  // copy-assignment operator: avoid modification of existing by another
  // constructor: avoid creation of multiple objects (singleton trickery)

  // can only be applied to object that is const qualified
  int get_value(int idx) const {
    return (idx < data.size()) ? data[idx] : -1e9;
  }
  void print_values() const {
    std::for_each(data.begin(), data.end(),
                  [](int v) { std::cout << v << "\n"; });
  }

  void set_value(int idx, int val) {
    (idx < data.size()) ? data[idx] = val : -1;
  }
};

void print_array_value(Array arr) {
  std::cout << "--------------" << "\n";
  arr.print_values();
  std::cout << "--------------" << "\n";
}
void print_array_reference(const Array &arr) {
  std::cout << "--------------" << "\n";
  arr.print_values();
  std::cout << "--------------" << "\n";
}

// NOTE: Array is returned by value
Array obtain_array(int size) { return Array(size); }

int main() {
  // pre-c++17: (cons. -> copy-cons. (rv : temp) -> copy-cons. (temp : a))
  Array a0 = obtain_array(5); // copy-constructor invoked

  // pre-c++17: (cons. -> copy-constructor -> copy-assignment)
  Array a;
  a = obtain_array(5);

  // lot of unneccesary copies: rv: temp & temp: a
  // (named) return value optimization
  // Compiler can construct result directly at final destination, wo copies
  // return Array(size); -> RVO / Array temp; return temp; -> NRVO

  a.set_value(1, 3);
  a.set_value(2, -32);

  Array b = a; // copy-constructor invoked
  b.set_value(3, 12334546);

  print_array_reference(a);
  print_array_reference(b);

  Array c = obtain_array(10);
  c = a; // copy-assignment operator invoked

  print_array_value(c); // pass by value: copy-constructor called
}
