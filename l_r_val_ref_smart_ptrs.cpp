/*
 * File: smart_ptrs_move_semantics.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-03-26
 * Description: L-r vals & refs and smart pointers
 * NOTE: keep it working & simple
 */

#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

int main() {

  // NOTE: l-val (addressable/identifer), r-val (not l), and l/r -val refs
  std::string s1{"Biggg"};    // s1: l-val, content: r-val
  std::string s2{"Bigggger"}; // s2: l-val, cntent: r-val

  std::string s3{s1 + s2}; // s3: l-val, s1+s2: r-val

  // NOTE: (DANGER: outlive the temporary r-val / const: lifetime boost)
  const std::string_view s3_5{s1 + s2}; // s3_5: l-val (NOT ref), s1+s2: r-val
  const std::string &s4{s1 + s2};       // s4: const l-val ref, s1+s2: r-val

  std::cout << "s3_5 lval ref: "
            << std::is_lvalue_reference<decltype(s3_5)>::value << "\n";
  std::cout << "s4 lval ref: " << std::is_lvalue_reference<decltype(s4)>::value
            << "\n";

  std::string &&s5{s1 + s2}; // s5: const r-val ref, s1+s2: r-val

  std::cout << "s5 rval ref: " << std::is_rvalue_reference<decltype(s5)>::value
            << "\n";

  // NOTE: s1+s2: r-value / effort made already / why copy again? / move (C++11)
  std::cout << s3 << "(object: copy) \n"
            << s4 << "(const alias: copy)\n"
            << s5 << " (alias: move)\n";

  /* NOTE:
   * const T& vs T&& : both capture r-vals then benefit?
   * const increases (temporary expr / r-val) lifetime, reference aliases it
   * if we need more references we can use the original reference, all const tho

   * NOTE:
   * resource: heap memory / file descriptor / socket / mutex / buffer
   * owner of the resource takes responsibility of set up & clean up
   * in case of heap memory, delete exactly once. None (mem leak) / Double free
   * move: no unnecessary copies (steal resources / maybe ownership transfer)
   * original: ends in a valid but unspecified state (destructor can be called)
   * pointer repointing behind the scenes?
   */
  std::cout << s3 << " (s3 before move) ||| ";

  std::string s6{std::move(s3)}; // steals from s3 (syntax sugar: preferred)
  // std::string s6 = static_cast<std::string &&>(s3); // equivalent to above

  std::cout << s3 << " (s3 after move) ||| " << s6 << " (s6 after move)\n";

  std::cout << std::endl; // clear buffer at program termination
  return EXIT_SUCCESS;
}
