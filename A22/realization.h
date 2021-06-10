#ifndef GUARD_realization_h
#define GUARD_realization_h
// realization.h
#include <iostream>
#include <array>
#include <vector>
#include <map>
#include "CF.h"

using F = CF<6>;

class H {
public:
  using value_type = std::array<F, 2>;
  value_type getVal() const { return val; }
private:
 value_type val;
};

std::ostream& operator<< (std::ostream&, const H& a);

using monomial = std::vector<std::pair<H, int> >;
bool operator< (const monomial&, const monomial&);
std::ostream& operator<< (std::ostream&, const monomial&);

class S {
public:
  using value_type = std::map<monomial, F>;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;
  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }
private:
  value_type val;
};

std::ostream& operator<< (std::ostream&, const S& v);

class X {
public:
  X() {}
  X(const H& _a, int _d) :a(_a), d(_d) {};
private:
  H a;
  int d;
};

S operator* (const X&, const S&);

#endif
