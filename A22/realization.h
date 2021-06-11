#ifndef GUARD_realization_h
#define GUARD_realization_h
// realization.h
#include <iostream>
#include <array>
#include <vector>
#include <map>
#include "CF.h"

const int CoxeterNum = 6;
using F = CF<CoxeterNum>;

// H is a 2-dim Vector sp with basis.
class H {
public:
  using value_type = std::array<F, 2>;
  H() {}
  H(const value_type& v) : val(v) {}
  H(const F& v1, const F& v2) : val({v1, v2}) {}
  value_type getVal() const { return val; }

  H proj(int n) const;
  F getProjVal(int n) const;

private:
 value_type val;
};

std::ostream& operator<< (std::ostream&, const H& a);

using Factor = std::pair<H, int>;

class Monomial {
public:
  using value_type = std::vector<std::pair<H, int> >;
  using const_iterator = value_type::const_iterator;

  Monomial() {}
  Monomial(const value_type& v) : val(v) { sort(); proj(); }
  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }
  value_type::size_type size() const { return val.size(); }

  F getCoeff() const;
  void unifyCoeff();
private:
  void sort();
  void proj();
  value_type val;
};

bool operator< (const Monomial&, const Monomial&);
std::ostream& operator<< (std::ostream&, const Monomial&);

class S {
public:
  using value_type = std::map<Monomial, F>;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;
  using Term = std::pair<Monomial, F>;

  S() {}

  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }

  std::pair<iterator, bool> insert(Term);

private:
  value_type val;
  Term& unify(Term&);
};

std::ostream& operator<< (std::ostream&, const S& v);

S& operator*(const Factor&, S&);

class Action {
public:
  using value_type = std::vector<Factor>;
private:
  S& operator*(S&);
  value_type val;
};

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
