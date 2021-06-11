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
  return true;
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

void Monomial::proj()
{
  using std::make_pair;
  for(value_type::iterator iter = val.begin();
      iter != val.end(); ++iter) {
    *iter = make_pair((iter -> first).proj(iter -> second), iter -> second);
  }
}

std::pair<S::iterator, bool> S::insert(Term x)
{
  auto ret = val.insert(unify(x));
  return ret;
}
