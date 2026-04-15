/*
 * File: l_r_values_cpp_con.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-04-07
 * Description: https://www.youtube.com/watch?v=XS2JddPq7GQ&t=80s
 * NOTE: keep it working & simple
 */

/*NOTE: expressions
 * value types help the compiler to process expressions. not a C++ feature
 * expressions: permutation of sub-expressions (literals & identifier)
 * eg. 3; int x; int x = 3; int y = x;
 * same indetifier can have different valueness based on usage/context
 *
 * NOTE: intro to l-r values
 * l-value: expression referring to an object (addressable region of memory)
 * r-value: expression that is not an l-value (does not refer to an object)
 * literals: 3, 3.14... (r-values), "alpha" (l-value: const static array)
 * l-value or r-value determination happens at compile-time (context)
 *
 * NOTE: valid operations
 * valid assignment: lhs: non-modifiable l-value, rhs: r-value (l to r convert)
 * valid built-in operator+: both operands can either be l-value / r-value
 * valid unary *: l-value -> pointer points to an object, by definition
 *
 * NOTE: why distinguish between l-r values?
 * as-if rule: compilers free to optimize as long as observable behavior same
 * as-if rule offers lot of freedom to compilers: .code (small) or .data
 * C/C++ insist to user: l-values occupy memory, non-class r-values do not
 *
 * NOTE: how to determine l-value vs r-value?
 * compiler decides the type: converts C/C++ to ASM; you should do too
 * if one absolutely needs the address to perform the operation: l-value
 *
 * NOTE: why distinguish: class(struct/enum/union) and non-class r-values?
 * struct S {int x,y;}; S s = {1, 4};
 * int i = s.y; // need base address to S. increment by 4 to get y
 * int i = S(1,4).y; // still need a base address to access members
 *
 * NOTE: ookly-dokly, where do reference come in?
 * a reference is essentially a const pointer (not a pointer to a const)
 * const pointer cannot be re-seated, can be modified, not auto de-referenced
 * de-reference leads to an l-value. provide cleaner-syntax than const pointers
 * reference of type T can only bind to an l-value of type T
 *
 * NOTE: why bother?
 * operator overloads are fancy functions: need to pass arguments (value, ptr)
 * pass-by-value fine for postfix++, >>, << (still not efficient if big)
 * pass-by-pointer is a must for operators like prefix++, +=
 * UDT x {3}; ++&x; -> incrementing address by 1? ambiguous resolution
 * C++ standard bans from the use of pointers on operator overloads: primitive
 *
 * NOTE: reference to const T
 * benefit of reference to const T: binds both l & r-values (temp object)
 * temp object need not be made from type T originally. Convertiable.
 * key benefit of this temp-binding: pass by reference of literals to fns
 * lifetime of temporary is extended for the duration of fn (pass-value like)
 *
 * NOTE: types of r-values: pure r-values & expiring r-values
 * prvalues: do NOT occupy storage, xvalues: do occupy storage
 * temp obj convert prvalues to xvalues using temp materialization conversion
 * STR(char const *); STR operator+(STR const &lo, STR const &ro);
 * string s = "hello"; string t = "world"; s = s + ", " + t;
 * l-val + string(char const *) -> r-val + l-val -> r-val + l-val -> r-val
 *
 * NOTE: C++11: types of references -> l-value & r-value references
 * l-value references are the same old conventional references
 * r-value references only bind to r-values (even r-val ref to const T)
 * r-value references: use the && operator. Used as fn params & return types
 * r-value references are used to implement move-semantics
 * s1 = s2; // s2 value is not expiring (assign from copy-assignment)
 * s1 = s2+s3; // s2+s3 is expiring (assign from move-assignement)
 * inside the move-constructor, an r-value reference acts as l-value reference
 */
