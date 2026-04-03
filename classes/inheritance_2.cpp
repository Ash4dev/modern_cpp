/*
 * File: inheritance_2.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-04-01
 * Description:
 * NOTE: keep it working & simple
 */

#include <iostream>
#include <string_view>

/* NOTE: dynamic dispatch (small indirection cost: not suitable for hot path)
 * inheritance graphs can grow really big: as application scales.
 * custom fns to deal with every specialized type, does NOT scale well.
 * exact type of the input is not known to the compiler, multiple reasons
 * maybe conditional return types / diff input source / malicious code
 *
 * NOTE: unified interface
 * generalized functions that can deal with the set of derived types appreciated
 * for eg. inheritance graph: b | d1 | d2 | ... | dk. Exploit "is-a" relation
 * f1 (dk-1,dk): f1(dk-1& ref) and f2 (d2...dk): f2(d2& ref)
 * why only pointer / reference ? => static type: Base, runtime type: Derived
 * if pass by value, derived may not fit (dynamic part lost: sliced away)
 *
 * NOTE: dynamic dispatch mechanism (compiler trick not part of C++ standard)
 * addition of virtual keyword registers the class as polymorphic (has vtable)
 * each object of such polymorhic class, receives a hidden pointer to its vtable
 * vtable is static array mapping between fn sigs (virtual mem fns) & fn ptr
 * it is made to point to the vtable of most-derived class of that object
 * it stores most-derived fn override as of yet. Static built at compile-time
 * when virtual fn called through base pointer / reference to derived object:
 * object's hidden vptr is looked at & the correponding fn ptr called
 *
 * NOTE: fn signature, override, final
 * fn name & parameter types & cv-qualifiers & reference qualifiers
 * TODO return type must be identical or covariant (ref / ptr only)
 * allows an overriding member function to return a narrower type. (is-a)
 * for eg. https://onlinegdb.com/uMWgl4NGw (is-a reln is must)
 * fn signature should exactly match to be considered an override
 * once a fn is marked virtual, all inherited overrides are implicitly virtual
 * never call virtual methods from constructors: in base, derived DNE!
 * never call virtual methods from destructors: in base, derived destroyed!
 *
 * NOTE: virtual destructors
 * make explicit destructors virtual (always) if inheritance is involved
 * sense: derived class allocates memory & a unified interface is used
 * Animal *ptr {new Cat}; delete ptr; destructor for static type (Animal) called
 * if the destructor were virtual, clean up is ensured for the derived classes
 * no need for override keyword, since all destructors have same signature
 *
 * NOTE: should we make all destructors virtual? what is the benefit-cost?
 * cost: class turned into polymorphic (just for destructor alone)
 * only allow virtual destructors for classes that are meant to be a base-class
 * old advice: make destructors for such classes protected and non-virtual
 * benefit: cannot delete base itself (through derived only), cost: base useless
 * modern advice: for non-base classes, mark the class itself as final
 * unrelated note: marking destructor virtual final (prevent inheritance)
 *
 * NOTE:
 * binding: associating properties to an identifier
 * early-binding: based on static (compile-time) information
 * late-binding: based on run-time information: in context of functions
 * direct fn call, non-virtual fn call (static) // fn ptr, virtual (run-time)
 * late-binding is slower than early-binding: indirection but flexibility
 * virtual table is a type of late-binding technique used for dynamic dispatch
 */
class Base {
public:
  // NOTE: use virtual keyword on virtual functions in a base-class
  virtual std::string_view getName() const { return "Base"; }
};

class Derived : public Base {
public:
  // NOTE: avoid usage of virtual keyword but override for compile-time check
  // override keyword incurs no performance penalty: good practice to use
  // final keyword prevents further inheritance / overriding at compile-time
  std::string_view getName() const override final { return "Derived"; }
};

class Derived2 : public Derived {
public:
  //  Declaration of 'getName' overrides a 'final' function. Not allowed
  // std::string_view getName() const override { return "Derived"; } // at;
};

int main() {
  Derived derived{};

  Base &rBase{derived}; // Derived type is known here, not always known
  std::cout << "rBase is a " << rBase.getName() << '\n';

  return 0;
}
