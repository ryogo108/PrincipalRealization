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
  return os << "OUTPUT OF MONOMIAL";
}

ostream& operator<< (ostream& os, const S& v)
{
  os << "[";
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    os << "["
       << iter -> first       // monomial
       << ", "
       << iter -> second      // coefficient
       << "]";
  }
  return os << "]";
}

bool operator< (const monomial& lhs, const monomial& rhs)
{
  return true;
}

S operator* (const X& x, const S& v)
{
  return v;
}
