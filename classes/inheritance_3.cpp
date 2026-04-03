/*
 * File: inheritance_3.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-04-03
 * Description:
 * NOTE: keep it working & simple
 */

/* NOTE: abstract classes
 * Class just acts as blueprint? Functionality unknown. Non-instantiable.
 * Naive: Base constructor protected. Derived, not main. Can still forget.
 * Alt: pure virtual functions. No definition in base. Derived must define.
 * Class with undefined >= 1 pure virtual functions is called an abstract class
 *
 * NOTE: interface classes
 * classes with only pure virtual functions. Naming convention: I{original_name}
 */
#include <iostream>
#include <string>
#include <string_view>

// class with only pure virtual functions + no state + virtual destructor
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
};

// NOTE: Animal is still abstract. Definition not inline. Fine.
std::string_view Animal::speak() const {
  return "Noise\n"; // Fail-safe
}

class AbstractCat : public Animal {};

class Cat : public Animal {
public:
  using Animal::Animal; // NOTE: inherit name custom constructor
  std::string_view speak() const override { return "Meow\n"; }
};

class Unknown : public Animal {
public:
  using Animal::Animal; // NOTE: inherit name custom constructor
  std::string_view speak() const override { return Animal::speak(); }
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
}
