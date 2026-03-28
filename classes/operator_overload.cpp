/*
 * File: classes_3.cpp
 * Author: Ashutosh Panigrahy
 * Created: 2026-03-28
 * Description: https://share.google/aimode/0aDDsbfmppk8r38ne
 * NOTE: keep it working & simple
 */

#include <iostream>
#include <istream>

class Vector3f {
  float x, y, z;

public:
  // Rule of 3 not followed (simplicity)
  Vector3f();
  Vector3f(float x, float y, float z);

  /* NOTE: operators are member functions part of the defined class
   * all non-static members receive this as the first argument implicitly
   */
  Vector3f operator+(const Vector3f &other) const; // const protects "this"
  Vector3f operator-() const;                      // unary -
  Vector3f operator-(const Vector3f &other) const; // binary -

  Vector3f operator++(int); // postfix ++
  Vector3f &operator++();   // prefix ++
  Vector3f &operator+=(const Vector3f &other);

  bool operator==(const Vector3f &other) const;

  // NOTE: copy-constructors for istream & ostream are deleted
  // as one cannot have multiple connections to a single resource
  std::istream &operator>>(std::istream &istream);

  /* NOTE: free function that is trusted by the class it is declared in
   * friendship of function is not mutual / transitive / inherited
   * not a member function, yet has access to pri/pro data of Vector3f
   * do NOT have access to "this": declared inside, defineed outside
   */
  friend std::ostream &operator<<(std::ostream &ostream,
                                  const Vector3f &vector);
};

// NOTE: can NOT use "this" as soon as constructor begins execution
Vector3f::Vector3f() : x{0}, y{0}, z{0} {}
Vector3f::Vector3f(float x, float y, float z) : x{x}, y{y}, z{z} {};

bool Vector3f::operator==(const Vector3f &other) const {
  const float EPSILON = 1e-5f; // Or std::numeric_limits<float>::epsilon()
  return (std::abs(x - other.x) < EPSILON && std::abs(y - other.y) < EPSILON &&
          std::abs(z - other.z) < EPSILON);
}

Vector3f Vector3f::operator+(const Vector3f &other) const {
  Vector3f ans;
  ans.x = this->x + other.x;
  ans.y = this->y + other.y;
  ans.z = this->z + other.z;
  return ans; // NRVO
}

Vector3f Vector3f::operator-() const {
  return Vector3f(-1 * this->x, -1 * this->y, -1 * this->z); // RVO
}

Vector3f Vector3f::operator-(const Vector3f &other) const {
  // direct implementation has no temporary, illustrative sake

  // return this->operator+(other.operator-); // explicit calls
  return *this + (-other); // implicit calls
}

Vector3f Vector3f::operator++(int) {
  Vector3f old = *this;
  this->x++;
  this->y++;
  this->z++;
  return old;
}

Vector3f &Vector3f::operator+=(const Vector3f &other) { // definition
  this->x += other.x;
  this->y += other.y;
  this->z += other.z;
  return *this;
}

std::istream &Vector3f::operator>>(std::istream &istream) {
  istream >> x >> y >> z;
  return istream;
}

std::ostream &operator<<(std::ostream &ostream, const Vector3f &vector) {
  // NOTE: x,y,z are private vars of Vector3f: Vector3f has trusted << as friend
  ostream << "( x: " << vector.x << " , y: " << vector.y << " , z: " << vector.z
          << " )";
  return ostream;
}
int main() {
  constexpr double d = 1.03;  // constant & known at compile-time (constexpr)
  Vector3f alpha{1.0f, d, 1}; // Narrowing protection (No compile)

  std::cout << "alpha: " << alpha << std::endl;
  /*
   * NOTE: most vexing parse: treated as function declaration instead of object
   * Vector3f beta(); // create function beta no params, with Vector3f rv
   * anything that can be interpreted as function declaration, WILL BE
   * pre C++ 11: avoided by leaving uninitialized
   * post C++11: direct list initialization
   */
  Vector3f beta{};

  std::cout << "Enter your vector3f coordinates: ";
  beta >> std::cin; // NOTE: reads backward & un-chainable

  (((std::cout << "addition: ") << (alpha + beta)) << std::endl);
  std::cout << "subtraction: " << (alpha - beta)
            << std::endl; // NOTE: reads right & chainable

  std::cout << "increment " << alpha << " with " << beta
            << " gives: " << (alpha += beta) << std::endl;

  std::cout << "alpha: " << alpha << std::endl;

  std::cout << "isequal: " << (Vector3f{1, 2, 1} == Vector3f{1, 1, 1})
            << std::endl;
}
