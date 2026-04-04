/*
 * File: inheritance_3.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-04-03
 * Description:
 * NOTE: keep it working & simple
 */

/* NOTE: general practice: inherit all classes from a non-copyable class
 * delete copy-constructor and delete copy-assignment operator
 * ensures that no object slicing occurs
 *
 * NOTE: abstract classes
 * Class just acts as blueprint? Functionality unknown. Non-instantiable.
 * Naive: Base constructor protected. Derived, not main. Can still forget.
 * Alt: pure virtual functions. No definition in base. Derived must define.
 * Class with undefined >= 1 pure virtual functions is called an abstract class
 *
 * NOTE: interface classes
 * classes with only pure virtual fns & virtual destructor. I{original_name}
 * generally, interface classes do not have any state & a virtual destructor
 *
 * NOTE: multiple inheritance
 * inheritance from independent inheritance chains is fine: no overlap (mixins)
 * if a method is not defined for the class but for its parents: ambiguous call
 * mixins ensure no errors: keep each mixin functionally independent of others
 * otherwise leads to a diamond inheritance structure. multiple ancestor copy
 * state modification through one of its parents, reflects change on single copy
 *
 * NOTE: virtual (base classes) inheritance: compiler trick
 * class Scanner: virtual public PoweredDevice {}
 * class Printer: virtual public PoweredDevice {}
 * class Copier: public Scanner, public Printer {}
 * in general, most derived class generates object for all virtual base classes
 * most derived class has the global inheritance structure: instantiate well
 * Scanner and Printer still have PoweredDevice constructor calls: own objects
 * constructor call order most derived constructor: (not applicable recursively)
 * virtual base, non-virtual base, derived classes
 * all virtual base classes have virtual table, even if NO virtual functions
 * ------------------- AVOID MULTIPLE INHERITANCE IF POSSIBLE -----------------
 *
 * NOTE: object slicing
 * classes should be passed by reference / address to allow for polymorphsim
 * std::vector<Base*> works fine. However, may not be preferred (personal)
 * std::vector<Base&> fails: since references cannot be reseated after init
 *
 * NOTE: frankenobject
 * where the derived object still exists: Derived d1, d2; Base &rb{d2}; rb = d1;
 * rb is always as alias for d2. Here, the value of d1 replaces aliased value
 * since operator= is not virtual, only Base part is copied to the alias
 * d2 has the base part of d1 and derived portion of d2. Problematic: stateful
 */
#include <functional> // reference_wrapper
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

class IOrganism {
public:
  virtual ~IOrganism() = default;

  virtual void birth() = 0;
  virtual void death() = 0;
};

class Animal : public IOrganism {
  std::string m_name{};

public:
  Animal() = default;
  Animal(std::string_view name) : m_name{name} {}
  virtual ~Animal() = default;

  // default implementations of IOrganism behavior
  void birth() override { std::cout << m_name << " is born\n"; }
  void death() override { std::cout << m_name << " has died\n"; }

  const std::string &get_name() const { return m_name; }
  virtual std::string_view speak() const = 0; // pure virtual function

  // NOTE: offload dynamic dispatch to another virtual fn called in friend
  virtual std::ostream &print(std::ostream &os) const {
    return os << "Animal\n";
  }

  // NOTE: only member functions can be virtual & even if possible diff object
  friend std::ostream &operator<<(std::ostream &os, const Animal &animal);
};

std::ostream &operator<<(std::ostream &os, const Animal &animal) {
  return animal.print(os);
}

// NOTE: Animal is still abstract. Definition not inline. Fine.
std::string_view Animal::speak() const {
  return "Noise\n"; // Fail-safe
}

class AbstractCat : public Animal {};

class Cat : public Animal {
public:
  using Animal::Animal; // NOTE: inherit name custom constructor

  std::string_view returnMeow() const { return "Meow\n"; }
  std::string_view speak() const override {
    // can delegate to member functions (naive->optimized etc)
    return this->returnMeow();
  }

  std::ostream &print(std::ostream &os) const override { return os << "Cat\n"; }
};

class Unknown : public Animal {
public:
  using Animal::Animal; // NOTE: inherit name custom constructor
  std::string_view speak() const override { return Animal::speak(); }

  std::ostream &print(std::ostream &os) const override {
    return os << "Unknown\n";
  }
};

int main() {
  Cat c{"Kitty"};
  Animal *o1 = &c;
  o1->birth();

  Unknown u{"Mystery"};
  Animal *o2 = &u;
  o2->birth();

  std::cout << o1->speak();
  std::cout << o2->speak();

  o1->death();
  o2->death();

  std::cout << "Species: " << *o1 << *o2 << std::endl;

  std::vector<Animal *> vec1;
  std::vector<std::reference_wrapper<Animal>> vec2; // mimics re-assignable ref
}
