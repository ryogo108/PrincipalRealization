#include <iostream>
#include "realization.h"

using std::ostream;

ostream& operator<< (ostream& os, const H& a)
{
  H::value_type val = a.getVal();
  return os << "(" << val[0] << ", " << val[1] << ")";
}

ostream& operator<< (ostream& os, const Monomial& m)
{
  os << "[";
  for(Monomial::const_iterator iter = m.begin();
      iter != m.end(); ++iter) {
    if(iter != m.begin()) os << ", ";
    os << iter -> first
       << "("
       << iter ->second
       << ")";
  }
  return os << "]";
}

ostream& operator<< (ostream& os, const S& v)
{
  using std::endl;
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    os << endl;
    if(iter != v.begin()) os << "+ ";
    os << "["
       << iter -> second      // coefficient
       << " * "
       << iter -> first       // Monomial
       << "]";
  }
  return os << endl;
}

bool operator< (const Monomial& lhs, const Monomial& rhs)
{
  if(lhs.size() != rhs.size()) return lhs.size() < rhs.size();
  Monomial::const_iterator it1 = lhs.begin();
  Monomial::const_iterator it2 = rhs.begin();
  while(it1 != lhs.end()) {
    if(it1 -> second != it2 -> second)
      return it1 -> second < it2 -> second;
    it1++;
    it2++;
  }
  return false;
}

S operator* (const X& x, const S& v)
{
  return v;
}

H H::proj(int n) const
{
  if((n - 1) % CoxeterNum == 0)
    return H(val[0], F(0));
  if((n + 1) % CoxeterNum == 0)
    return H(F(0), val[1]);
  return H(F(0), F(0));
}

F H::getProjVal(int n) const
{
  if((n - 1) % CoxeterNum == 0)
    return val[0];
  if((n + 1) % CoxeterNum == 0)
    return val[1];
  return F(0);
}

void Monomial::sort()
{
  std::sort(val.begin(), val.end(), [](const Factor& lhs, const Factor& rhs){
    return lhs.second < rhs.second;
  });
}

void Monomial::proj()
{
  using std::make_pair;
  for(value_type::iterator iter = val.begin();
      iter != val.end(); ++iter) {
    *iter = make_pair((iter -> first).proj(iter -> second), iter -> second);
  }
}

F Monomial::getCoeff() const
{
  F ret = F(1);
  for(Monomial::const_iterator iter = val.begin();
      iter != val.end(); ++iter) {
    ret *= (iter -> first).getProjVal(iter -> second);
  }
  return ret;
}

void Monomial::unifyCoeff()
{
  for(value_type::iterator iter = val.begin();
      iter != val.end(); ++iter) {
    if((iter -> second - 1) % 6 == 0) {
      (iter -> first) = H(F(1), F(0));
      continue;
    }

    if((iter -> second + 1) % 6 == 0) {
      (iter -> first) = H(F(0), F(1));
      continue;
    }

    (iter -> first) = H(F(0), F(0));
  }
}

std::pair<S::iterator, bool> S::insert(Term x)
{
  auto ret = val.insert(unify(x));
  return ret;
}

S::Term& S::unify(Term& x)
{
  x.second *= x.first.getCoeff();
  x.first.unifyCoeff();
  return x;
}

S& S::operator+=(const S& v)
{
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    value_type::iterator it = val.find(iter -> first);
    if(it != val.end()) {
      it -> second += iter -> second;
    }
    else
      val.insert(*iter);
  }

  omit();

  return *this;
}

S& operator*(const Action& a, S& v)
{
  for(Action::const_reverse_iterator riter = a.rbegin();
      riter != a.rend(); ++riter) {
    v += (*riter) * v;
  }
  return v;
}
