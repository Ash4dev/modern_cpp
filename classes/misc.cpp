/*
 * File: misc.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-04-04
 * Description:
 * NOTE: keep it working & simple
 */

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <vector>

/* NOTE: const member functions
 * current object's this is treated as a constant; cannot change object state
 * static member functions can be called as belong to the class not object
 * non-const member fn on other objects can certainly be called
 *
 * NOTE: initializer_list
 * initializer_list: curly brace initialization; prevents narrowing
 * initializer_list constructors are preferred over vanilla constructors
 * if do NOT exist, then curly brace is forced to go with vanilla match cons
 *  https://www.learncpp.com/cpp-tutorial/stdinitializer_list/
 *
 * NOTE: object relationships: part-of, has-a, uses-a, depends-on, and member-of
 * object composition (has-a): building complex objects from simpler ones
 * structs / classes: examples of object composition aka composite types
 * good practice: each composite type accomplishes a single task. (self focused)
 * association: relationship between unrelated objects
 *
 * NOTE: Questions
 * is B part of A? No: association
 * is B integral to A's identity? Without, breaks. No: aggregation, Yes: comp
 * OOPS models meaning, not syntax: same objects can have different relations
 *
 * NOTE: types of object composition: composition (strong)
 * composition: object-member reln (member, lifetime dep, 1 owner, unaware)
 * composition does NOT restrict transferability. Move allowed. Owner changes.
 * composition: "part-of" relationships (o1 phsically contained in o2)
 * eg. normal data members + dynamic-like with RAII with object
 * if composition is possible for the relationship, go for it (simple)
 *
 * NOTE: types of object composition: aggregation (weak)
 * aggregation: object-member reln (member, lifetime indp, >=1 owner, unaware)
 * aggregation is not responsible for creation / deletion of parts
 * person dies: heart dies / car dies: engine can be fit in same model car
 * eg. references / pointers to objects created outside scope of class
 * the references / pointers will be destroyed (composition), not free ref obj
 *
 * NOTE: implement the smallest relationship that meets your use-case
 * same: car-engine (composition: car-racing / aggregation: mechanic game)
 * composition and aggregation are often mixed together to obtain meaninful obj
 *
 * NOTE: object-relationship: association (uses-a)
 * object-member(unrelated, lifetime indp, >=1 owners, may/may not know)
 * bugers-fries: association combo meal-burgers/fries: composition
 */

struct S1 {
  // NOTE: padding introduces uniformity: ease up reads for processor
  // If in between 2-3-4-5 -> would need 2 processor reads for float
  bool b;  // 1 + 3 pad (must begin at multiple of 1)
  float f; // 4 +0 pad (must begin at multiple of 4)
  short s; // 2 + 2 pad (size of structmult of largest alignment reqmt)
}; // 12 bytes

struct S2 {
  // NOTE: arrange from largest to smallest to minimize struct padding
  float f; // 0-3
  short s; // 4-5
  bool b;  // 6 + 1 pad
}; // 8 bytes

class UDT {
  // initializer_list is not a good choice for data member
  // just a view: pointer to temporary data with size
  // pass by value in functions is fine: just a view
  std::vector<int> m_data;

  // NOTE: in-class initializer (constructor > in-class > value-init)
  // logic: prefers the most-explicit initialization
  int alpha{0};
  std::string name;

  // prevents ODR violation: but manual work for linker to deduplicate
  inline static int s_my_var = 10;                 // C++17
  static constexpr std::string_view s_view{"LOl"}; // implicitly inline

public:
  int static s_instance; // inform compiler that variable exists
  int static const s_const_instance = 7;

  // static member function can NOT have const qualifier
  int static get_s_instance() {
    // cannot access this keyword
    return s_instance * s_const_instance;
  }

  UDT() = default;
  UDT(int a, int b, int c);
  UDT(int a, int b);
  UDT(std::initializer_list<int> data) : m_data{data} {
    // copy value into vector for later use
    std::cout << "initializer_list constructor called\n";
  }
  void print_values() const {
    for (auto &ele : m_data) {
      std::cout << ele << ", ";
    }
    std::cout << std::endl;
  }
  std::size_t size() const { return m_data.size(); }
  ~UDT() = default;
};

// static can only be applied in class definition, scope limited to class
// static members do NOT belong to the class, it is the scope that is limited
// if not defined in file-scope (ONLY like a global variable), linker error
// shared by all objects -> not part of object -> global var - class scope
// instance.static_member allowed, but not part of the object
// non-template class: define in corr .cpp file / not in header (unless inline)
int UDT::s_instance = 0; // acutal memory alloc (not subject to access control)

// NOTE: mixing: delegate ctr & mem-val BAD (prevent double object init)
// General practice to create a private principal constructor with appt vals
UDT::UDT(int a, int b, int c) : alpha{a * b / c}, name{"hello"} {}
UDT::UDT(int a, int b) : UDT(a, b, 1) {
  std::cout << " initializer for a delegating constructor must appear alone\n";
  std::cout << "runs after the delegated constructor\n";
} // NOTE: delegated constructor

int main() {
  // int x {3.3}; // prevents narrowing

  std::cout << "size alignment of S1: " << alignof(S1) << std::endl;
  std::cout << "size alignment of S2: " << alignof(S2) << std::endl;

  UDT v1{1, 2, 3}; // data is temporary & destroyed at expression end
  v1.print_values();

  UDT v2{}; // value-init: zero-initialized since default constructor invoked
  std::cout << v2.size() << std::endl;

  UDT v3(1, 2, 3); // goes for (int,int,int) -> can call all mem fns

  const UDT v4(1, 1, 1); // can only call const member functions
}
