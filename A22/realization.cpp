#include <iostream>
#include "realization.h"

using std::ostream;

ostream& operator<< (ostream& os, const H& a)
{
  H::value_type val = a.getVal();
  return os << "(" << val[0] << ", " << val[1] << ")";
}

ostream& operator<< (ostream& os, const monomial& m)
{
  os << "[";
  for(monomial::const_iterator iter = m.begin();
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
       << iter -> first       // monomial
       << "]";
  }
  return os << endl;
}

bool operator< (const monomial& lhs, const monomial& rhs)
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

std::pair<S::iterator, bool> S::insert(S::value_type::value_type x)
{
  return val.insert(x);
}
