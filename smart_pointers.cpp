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
   * shared: - / automatic (RAII) / alive (an owner is)   / free iff ref_cnt = 0
   * P -> C, C -> P / ref-cnt init at 1 / ref-cnt++ for each / cyclic dependency
   * weak: non-owning (ref-cnt += 0)
   *
   * NOTE: nullptr dereference STILL PERSISTS
   * weak: must lock() and null-check first
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
}
