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

  H& operator+=(const H&);
  H operator-() const;
  F innerProd(const H& h) const { return F(3) * (val[0] * h.val[1] + val[1] * h.val[0]); };

private:
 value_type val;
};

std::ostream& operator<< (std::ostream&, const H& a);
bool operator==(const H&, const H&);
H operator+(H, const H&);
H operator-(H, const H&);
H operator*(const F&, const H&);

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
  Core(const value_type& v) : val(v) { proj(); }
  Core(const Factor& f) { push_back(f); }
  Core(const const_iterator& b, const const_iterator& e) : val(b, e) {};
  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }
  const_reverse_iterator rbegin() const { return val.rbegin(); }
  const_reverse_iterator rend() const { return val.rend(); }
  value_type::size_type size() const { return val.size(); }

  void push_back(const Factor&);
  const_iterator erase(const_iterator&);

  F getCoeff() const;
  void unifyCoeff();
protected:
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
  using size_type = value_type::size_type;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;
  using Term = std::pair<Monomial, F>;

  S() {}

  iterator begin() { return val.begin(); }
  iterator end() { return val.end(); }
  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }

  std::pair<iterator, bool> insert(Term);
  const_iterator find(const Monomial& m) const { return val.find(m); };
  size_type size() const { return val.size(); };

  S& operator+=(const S&);
  S& operator-=(const S&);
private:
  value_type val;
  Term& unify(Term&);
  void omit();
};

std::ostream& operator<< (std::ostream&, const S& v);

bool operator==(const S&, const S&);
S operator+(S, const S&);
S operator-(S, const S&);
S operator*(const F&, const S&);
S operator*(const Factor&, const S&);

class Action: public Core{
public:
  Action() {}
  Action(const value_type& v) : Core(v) {}
  Action(const Factor& f) : Core(f) {}
  Action(const const_iterator& b, const const_iterator& e) : Core(b, e) {}
};

Action operator*(Action, const Action&);
S operator*(const Action&, const S&);

class Actions {
public:
  using value_type = std::map<Action, F>;
  using size_type = value_type::size_type;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;
  using Term = std::pair<Action, F>;

  Actions() {}
  Actions(const Action& a) { insert(std::make_pair(a, F(1))); }
  Actions(const Factor& f) : Actions(Action(f)) {}
  Actions(const F& a) { insert(std::make_pair(Action(), a)); }

  iterator begin() { return val.begin(); }
  iterator end() { return val.end(); }
  const_iterator begin() const { return val.begin(); }
  const_iterator end() const { return val.end(); }
  bool empty() const { return val.empty(); }
  std::pair<iterator, bool> insert(Term);
  size_type size() const { return val.size(); };
  const_iterator find(const Action& a) const { return val.find(a); };
  F& operator[](const Action& a) { return val[a]; }

  Actions& operator+=(const Actions&);
  Actions& operator-=(const Actions&);
  Actions& operator*=(const Actions&);
private:
  Term& unify(Term&);
  void omit();
  value_type val;
};

Actions straighten(const Action&);

std::ostream& operator<<(std::ostream&, const Actions&);

Actions operator+(Actions, const Actions&);
Actions operator-(Actions, const Actions&);
Actions operator*(const F&, const Actions&);
Actions operator*(Actions, const Actions&);

bool operator==(const Actions&, const Actions&);

S operator*(const Actions&, const S&);

class Operators {
public:
  using size_type = std::size_t;
  static const size_type DEG0 = 10;
  static const size_type MAX_DEG = 2 * DEG0 + 1;
  static const size_type MIN_DEG = 0;
  using value_type = std::array<Actions, MAX_DEG>;

  Operators() { create(); }
  Operators(const F& f) { create(f); }

  value_type::value_type& operator[](int n) { return val[n + DEG0]; }
  const value_type::value_type& operator[](int n) const { return val[n + DEG0]; }
  Operators& operator +=(const Operators&);
  friend std::ostream& operator<< (std::ostream&, const Operators&);
private:
  void create();
  void create(const F&);
  value_type val;
};

Operators operator*(const F&, const Operators&);
Operators operator*(const Operators&, const Operators&);

const Operators E_plus(const H& a);
const Operators E_minus(const H& a);
const Actions X(const H&, int);

H nu(int n, const H&);
F epsilon_2(const H&, const H&);

const Actions comX(const H&, int, int);

#endif
