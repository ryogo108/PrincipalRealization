#ifndef GUARD_realization_h
#define GUARD_realization_h
// realization.h
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

class S {
  using monomial = std::vector<std::pair<H, int> >;
private:
  std::map<monomial, F> val;
};

std::ostream& operator<< (std::ostream&, const S& v);

class X {
public:
  X() {}
  X(const H&, int);
private:
  H a;
  int d;
};

S operator* (const X&, const S&);

#endif
