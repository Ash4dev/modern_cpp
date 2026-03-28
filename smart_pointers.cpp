/*
 * File: smart_pointers.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-03-26
 * Description: Explore smart pointer (unique, shared, weak)
 * NOTE: keep it working & simple
 */

#include <iostream>
#include <memory> // smart pointers

class Custom {
  char type;

public:
  Custom() = default;
  Custom(char c) {
    this->type = c;
    std::cout << this->type << " con" << std::endl;
  }
  void hello() { std::cout << "hello\n"; }
  ~Custom() { std::cout << this->type << " des" << std::endl; }
};

int main() {
  /* NOTE: problems with raw vanilla pointer
   * nullptr dereference / memory leaks / dangling pointer / double delete
   * sharing by default: no explicit ownership concept
   *
   * NOTE: smart pointer solution to respectively: (complex: unique -> shared)
   * unique: - / automatic (RAII) /  out of scope: delete / no copies allowed
   * std::move -> deletes old resource / steal mover's resource
   *
   * NOTE: ref_count evolution: important to move ahead with weak pointer
   * shared: - / automatic (RAII) / alive (an owner is)   / free iff ref_cnt = 0
   * tip: draw a diagram for each control block to understand better.
   * ref-cnt: is for the control block. Each pointer to a CB, has same ref
   * count. When sp.ref_count() is first resolves to the CB, then reported.
   * sp.reset() / sp = nullptr. Points back to null. Goes to zero
   * after these. int * raw = sp.get(); ref_count unchanged. user must delete.
   * pass by value: ref_count++, pass by reference: as is
   * https://chatgpt.com/g/g-p-69673c1795888191a19a1ea6885798b9-systems-programming/shared/c/69c60a05-5774-8323-8cf6-b1f3705f469f?owner_user_id=user-CJUqFrsqykR5uAe3R4C6mge3
   *
   * NOTE: nullptr dereference STILL PERSISTS
   * weak: non-owning (ref-cnt += 0)
   * resolve cyclic dependency issue. n1: o1, n2:o2, n1->next:o2, n2->next:o1
   * weak: wp.lock() and null-check first / creates temp shared_ptr
   * wp = sp; if sp.reset(), wp.lock() return nullptr
   */

  // NOTE: unique pointer: ownership / lifetime -> deterministic
  std::cout << "block depth: 0" << std::endl;

  std::unique_ptr<Custom> unq_cs = std::unique_ptr<Custom>(new Custom('u'));
  {
    // NOTE: standard only supports default / aggregate init
    std::cout << "block depth: 1" << std::endl;

    std::unique_ptr<Custom[]> unq_cs_arr =
        std::unique_ptr<Custom[]>(new Custom[1]{'u'});
    {
      std::cout << "block depth: 2" << std::endl;

      std::unique_ptr<Custom[]> unq_cs_arr_better_syntax =
          std::make_unique<Custom[]>(1);
      for (int i = 0; i < 1; ++i) // NOTE: dynamic size handling
        unq_cs_arr_better_syntax[i] = Custom('u');

      // DELETED: unq_cs_arr = unq_cs_arr_better_syntax;
      unq_cs_arr_better_syntax = std::move(unq_cs_arr); // ALLOWED

      std::cout << "block depth: 2" << std::endl;
    }

    std::cout << "block depth: 1" << std::endl;
  }

  // NOTE: shared pointer: ownership / lifetime -> non-deterministic
  std::shared_ptr<Custom> shrd_cs_1 = std::make_shared<Custom>('s');
  // NOTE: use_count is only to be use in non-multithreded environments
  // atomic (reports snapshot value), but may not be valid immediately after
  std::cout << "ref cnt for shrd_cs : (1) " << shrd_cs_1.use_count() << "\n";

  {
    auto shrd_cs_2 = shrd_cs_1;
    std::weak_ptr<Custom> shrd_cs_3 = shrd_cs_1;

    // NOTE: number of references (idea) to the shared memory block (resource)
    std::cout << "ref cnt for shrd_cs : (1) " << shrd_cs_1.use_count()
              << " (2) " << shrd_cs_2.use_count() << " (3) "
              << shrd_cs_3.use_count() << "\n";
  }
  std::cout << "ref cnt for shrd_cs : (1) " << shrd_cs_1.use_count() << "\n";

  // on destroy: shrd_cs_2 destroyed -> 1, shrd_cs_1 destroyed -> 0
  std::cout << "block depth: 0" << std::endl;

  // NOTE: weak pointers
  std::weak_ptr<Custom> weak_cs;
  weak_cs = shrd_cs_1; // does NOT raise ref count

  // NOT thread safe
  if (not weak_cs.expired()) {
    // weak_cs cannot dereference since not an owner
    std::shared_ptr<Custom> temp = weak_cs.lock(); // temp ref_count++
    temp->hello();
  }
  // temp goes out of scope

  if (auto temp = weak_cs.lock()) {
    temp->hello();
  }

  return 0;
}
