/*
 * File: big_5.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-03-29
 * Description: Rule of 5
 * NOTE: keep it working & simple
 */
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#define SIZE 3

class IntArray {
private:
  std::string name;
  int *data;

public:
  // NOTE: every constructor must leave object in valid destructable state
  IntArray();                                 // default constructor
  IntArray(const IntArray &other);            // copy-constructor
  IntArray(IntArray &&source);                // move-constructor
  IntArray &operator=(const IntArray &other); // copy-assignment operator
  IntArray &operator=(IntArray &&source);     // move-assignment operator
  ~IntArray();                                // destructor

  // NOTE: restricts copy init or copy list init to class type not arg match
  // for the time being assume IntArray also has a constructor from int
  // copy-init: IntArray a = 10; copy-list-init: IntArray a = {10};
  // prevents implicit conversions (as copy-init / copy-list-init) used
  // allows direct-init (IntArray a(10)) / direct-list-init (IntArray a{10})
  // return type does not match, cannot use explicit constructor(pass-by-v)

  // NOTE: practical mental rule: if init with =, explicit constructors ignored
  // make any constructor that will accept a single argument explicit by default
  // Do not make copy/move cons. explicit, as these do not perform conversions.

  // void fn (IntArray): fn(IntArray("name")) WORKS but fn("name") does NOT
  // "name" -> IntArray -> fn is NOT allowed anymore with explicit keyword
  explicit IntArray(std::string name); // custom constructor

  // static_cast<bool> (arr) WORKS bool b = arr; FAILS
  // void fn(bool): fn(arr) will NOT work
  explicit operator bool() const;
  // if (arr) / while(arr) / for (; ();) / logical operators: boolean contexts
  // boolean contexual bool conversions (special)
};

// default lead to unitialized data when freed lead to segmentation fault
IntArray::IntArray() : name{}, data{nullptr} {}

IntArray::IntArray(std::string name) : name{name}, data{new int[SIZE]} {
  std::cout << name << " : constructor called\n";
}

IntArray::IntArray(const IntArray &other) {
  std::cout << "(deep) copy constructor operator called\n";
  // allocate new memory first
  int *new_data = new int[SIZE];
  std::copy(other.data, other.data + SIZE, new_data);

  // assign the new memory to this
  name = other.name + "_copy";
  data = new_data;
}

IntArray &IntArray::operator=(const IntArray &other) {
  std::cout << "(deep) copy assignment operator called\n";
  // prevent self assignment
  if (this == &other) {
    return *this;
  }

  // allocate new memory first
  int *new_data = new int[SIZE];
  std::copy(other.data, other.data + SIZE, new_data);

  // get rid of old this data
  delete[] data;

  // assign the new memory to this
  name = other.name + "_copy";
  data = new_data;
  return *this;
}

IntArray::~IntArray() {
  std::cout << "destructor called\n";
  delete[] data;
}

// NOTE: on move, lifetimes are moved from source to this
IntArray::IntArray(IntArray &&source)
    : name{std::move(source.name)}, data{source.data} {
  std::cout << "move constructor operator called\n";
  // invalidate source resources
  source.data = nullptr;
  // source need not be destructed manually (called when out of scope)
}

IntArray &IntArray::operator=(IntArray &&source) {
  std::cout << "move assignment operator called\n";
  // prevent self assignment
  if (this == &source) {
    return *this;
  }

  // clear resources from this
  name.clear();
  delete[] data;

  // steal resources from source
  name = std::move(source.name);
  data = source.data;

  // invalidate source resources
  source.data = nullptr;

  return *this;
}

// explicit keyword is ONLY used in the declaration
IntArray::operator bool() const { return (this->data != nullptr); }

IntArray foo() {
  IntArray arr{"yo"};
  // NOTE: should not there be a copy-constructor call? if thought naively
  // RVO / NRVO are compiler optimization mechanisms, not move related at all
  // RVO: no copy/move at all, Move: cheap transfer, Copy: expensive duplicate
  // RVO is for unnamed objects, while NRVO is for local variables (here)
  // RVO is guaranteed copy-elision by C++17 standard, while NRVO is NOT
  // Move constructor is a fallback when NRVO fails
  return arr;
}

int main() {
  // NOTE: stack (frames) operate as LIFO: first created, last destroyed

  std::vector<IntArray> vec(SIZE);
  for (int i = 0; i < SIZE; ++i) {
    vec[i] = std::move(IntArray(std::to_string(i))); // move-assignment
  }

  std::cout << "-------------------------------\n";
  IntArray a0{"lol"};          // custom constructor called
  IntArray a1 = std::move(a0); // move-constructor called

  IntArray a2{foo()};

  // NOTE: track number of heap allocations / leaks using valgrind
  return 0;
}
