/*
 * File: classes.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-03-27
 * Description: Explore classes
 * NOTE: keep it working & simple
 */

#include <algorithm>
#include <iostream>

// NOTE: program's classes: program-defined types from fundamental types
// it can have a variety of attributes and methods associated with it

// NOTE: need to know basis for external (access modifiers: abstraction)
// combine all related fields together (encapsulation)
// parent-children relationship (inheritance)
// behavior decision of method call: compile-time / run-time (polymorphism)

// NOTE: only default constructor / destructor is provided by C++ standard
// other constructors: copy constructor, copy-assignment operator
// destructors can NOT be overloaded: resolution impossiblle (out of scope)

// NOTE: avoid custom constructors whenever and wherever it is possible
// if custom needed and the data members are not easily copyable (dyn mem..)
// good to define: custom destructor / copy-constructor / copy-assignment
// Rule of 3: implicit constructors generate shallow copies by default
// Rule of BIG 2: If RAII is followed by data members, destructor is optional

// NOTE: classes and structs are functionally equivalent (default access mode)
// Byte padding is performed to word-align for ease of preprocessor access
// Convention: Use struct in a C-fashion and classes for everything else

class Array {
private:
  int *data;

public:
  ~Array() { delete[] data; }

  Array() {
    data = new int[5];
    int i{};
    std::generate(data, data + 5, [&i]() mutable { return i * i++; });
  }

  Array(const Array &other) {
    // other.data is private member of other. How can it be accessed?
    // access modifiers restrict visibility for external code (outside class)
    std::cout << "copy-constructor called" << "\n";

    // obtain a new memory block & fill it appropriately
    int *new_data = new int[5];
    std::copy(other.data, other.data + 5, new_data);

    // assign the new data
    data = new_data;
  }

  Array &operator=(const Array &copy) {
    std::cout << "copy assignment operator called" << "\n";

    // prevent copy to itself
    if (this == &copy)
      return *this;

    // obtain new memory & fill appropriately with copy's data
    int *new_data = new int[5];
    std::copy(copy.data, copy.data + 5, new_data);

    // NOTE: can direct copy HERE, NOT when size is variable: FRESH START
    delete[] data;

    // assign the new data
    data = new_data;
    return *this;
  }

  // can only be applied to object that is const qualified
  int get_value(int idx) const { return (idx < 5) ? data[idx] : -1e9; }

  void set_value(int idx, int val) { (idx < 5) ? data[idx] = val : -1; }
  void print_values() {
    std::for_each(data, data + 5, [](int v) { std::cout << v << "\n"; });
  }
};

int main() {
  Array a;
  a.set_value(1, 3);
  a.set_value(2, -32);

  // MyClass newObj = oldObj; or MyClass newObj(oldObj);
  Array b = a; // copy-constructor invoked
  b.set_value(3, 12334546);

  /* NOTE: extent of copy: basis of b's changes reflected on a
   * int data[5]: indepenedent (deep) copies, point to separate memory blocks
   * int *data: dependent (shallow) copies, point to the same memory block
   *
   * NOTE: issue with shallow copy:
   * problem : deallocation leads to double free, crashes the program at
   * run-time solution: create another memory block, similar to stack
   */
  a.print_values();
  std::cout << "--------------" << "\n";
  b.print_values();
  std::cout << "--------------" << "\n";

  // existingObj1 = existingObj2;.
  Array c;
  c = a; // copy-assignment operator invoked
  c.print_values();
}
