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
bool operator==(const H&, const H&);

using Factor = std::pair<H, int>;
std::ostream& operator<< (std::ostream&, const Factor&);
Factor proj(const Factor& f);
F getCoeff(const Factor& f);
Factor unifyCoeff(const Factor& f);

class Core {
public:
  using value_type = std::vector<Factor>;
  using const_iterator = value_type::const_iterator;
  using const_reverse_iterator = value_type::const_reverse_iterator;

  Core() {}
  Core(const value_type& v) : val(v) { sort(); proj(); }
  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }
  const_reverse_iterator rbegin() const { return val.rbegin(); }
  const_reverse_iterator rend() const { return val.rend(); }
  value_type::size_type size() const { return val.size(); }

  void push(const Factor&);
  const_iterator erase(const_iterator&);

  F getCoeff() const;
  void unifyCoeff();
protected:
  void sort();
  void proj();
private:
  value_type val;
};


class Monomial: public Core {
public:
  Monomial() {}
  Monomial(const value_type& v) : Core(v) {};
};

bool operator< (const Core&, const Core&);
std::ostream& operator<< (std::ostream&, const Monomial&);

class S {
public:
  using value_type = std::map<Monomial, F>;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;
  using Term = std::pair<Monomial, F>;

  S() {}

  iterator begin() { return val.begin(); }
  iterator end() { return val.end(); }
  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }

  std::pair<iterator, bool> insert(Term);

  S& operator+=(const S&);
private:
  value_type val;
  Term& unify(Term&);
  void omit();
};

std::ostream& operator<< (std::ostream&, const S& v);

S operator*(const F&, const S&);
S operator*(const Factor&, const S&);

class Action: public Core{
public:
  Action() {}
  Action(const value_type& v) : Core(v) {}
};

S operator*(const Action&, const S&);

class Actions {
public:
  using value_type = std::map<Action, F>;
  using const_iterator = value_type::const_iterator;

  Actions() {}
  Actions(const Action& a) { create(a); }
  Actions(const Factor& f) : Actions(Action(f)) {}

  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }
  bool empty() const { return val.empty(); }
private:
  void create(const Action& a);
  value_type val;
};


S operator*(const Actions&, const S&);

const std::size_t DEG0 = 2;
const std::size_t MAX_DEG = 2 * DEG0 + 1;
const std::size_t MIN_DEG = 0;
using Operators = std::array<Actions, MAX_DEG>;

Actions E_plus(const H& a, int n);
Actions E_minus(const H& a, int n);

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
