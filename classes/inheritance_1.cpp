/*
 * File: inheritance_1.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-03-30
 * Description:
 * NOTE: keep it working & simple
 */
#include <iostream>
#include <string_view>

/* NOTE: 3 modes of access specifiers: pub, pro, pri
 * access specifiers indicate availability of info from external code
 * pub: anywhere access, pro: friends, derived classes, pri: nowhere

 * NOTE: 3 modes of access specifiers: pub, pro, pri
 * pri info is never shared with derived; pub & pro shared
 * IMP: pub: pub, pro members of base retain original access levels in derived
 * pro: pub, pro members of base are pro members of derived
 * pri: pub, pro members of base are pri members of derived

 * NOTE: dk class object is composed of multiple parts: base | d1 .. | dk
 * when relevent constructor (dk) called, two activites need action:
 * call apt parent constructor (default) & member attributes (dk) are init
 * constructor call can ONLY be made to its immediate parent in class hierarchy
 * this leads to a recursive sequence: dk is composed of multiple objects levels
 * intuitive logic behind recrsion: build root object first, then specialize
 * destruction proceeds recursive but in the opposite direction (dk |..d1|b)
 *
 * NOTE: function resolution: happens first on the basis of name & then args
 * only the fns with the same target name at the same class level are considered
 * argument matching happens on these functions, even if better exist above
 * start from most derived & proceed upwards if allowed with using Base::name
 *
 * NOTE: mixins are highly focused subclasses with orthogonal properties
 * meant to be used as plugins, and not on their own. "has-a" capability
 * mixins are still combined with a regular multiple inheritance mechanism
 * if mixin tries to introduce identity / state, it is NOT a mixin
 * inheritance: vertical, mixin: horizontal
 *
 * TODO: CRTP (Curiously Recurring Template Pattern) -> learn templates
*/
class Base {
public:
  Base() { std::cout << "base default cons\n"; }
  Base(std::string_view) { std::cout << "base custom cons\n"; }
  ~Base() { std::cout << "base default dest\n"; }

  void print(int) { std::cout << "Base::print(int)\n"; }
  void print(double) { std::cout << "Base::print(double)\n"; }

  int cat() {
    std::cout << "Base::cat()\n";
    return 0;
  }

protected:
  void do_some(int) { std::cout << "Base::do_some(int)\n"; }
  void do_some(float) { std::cout << "Base::do_some(float)\n"; }
};

class Derived : public Base {
public:
  Derived() { std::cout << "derived default cons\n"; }
  Derived(std::string_view str) : Base{str}, name{str} {
    std::cout << "derived custom cons\n";
  }
  ~Derived() { std::cout << "derived default dest\n"; }

  // NOTE: compile-time polymorphism
  using Base::print; // allows Base fns for resolution alongside
  void print(float) { std::cout << "Derived::print(float)\n"; }

  // NOTE: all do_some is public wrt Derived (access changed from protected)
  // does not alter behavior of Base in any way, in fact no one can
  // if vice-versa: pub -> pri: typecast to base and access (hacking)
  using Base::do_some;

  // NOTE: this function is made inaccessible (NOT avilable at any cost)
  int cat() = delete;

private:
  std::string_view name;
};

int main() {
  {
    Derived d1{"yooooo"};
    d1.do_some(3);
  }
  std::cout << "-----------------------\n";
  {
    Derived d2;
    // d2.cat(); // Attempt to use a deleted function
    static_cast<Base>(d2).cat(); // fine! (better IMO)
    d2.Base::cat();              // fine!
  }
}
