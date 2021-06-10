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
private:
 std::array<F, 2> val;
};

std::ostream& operator<< (std::ostream&, const H& a);

using monomial = std::vector<std::pair<H, int> >;
bool operator< (const monomial&, const monomial&);

class S {
private:
  std::map<monomial, F> val;
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
