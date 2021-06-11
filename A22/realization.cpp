#include <iostream>
#include "realization.h"

using std::ostream;
using std::make_pair;

ostream& operator<< (ostream& os, const H& a)
{
  H::value_type val = a.getVal();
  return os << "(" << val[0] << ", " << val[1] << ")";
}

ostream& operator<< (ostream& os, const Factor& f)
{
  return os << f.first << "(" << f.second << ")";
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

bool operator== (const H& lhs, const H& rhs)
{
  return lhs.getVal() == rhs.getVal();
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

Factor proj(const Factor& f)
{
  Factor ret = Factor(f.first.proj(f.second), f.second);
  return ret;
}

F getCoeff(const Factor& f)
{
  return f.first.getProjVal(f.second);
}

Factor unifyCoeff(const Factor& f)
{
  if((f.second - 1) % 6 == 0)
    return Factor(H(F(1), F(0)), f.second);
  if((f.second + 1) % 6 == 0)
    return Factor(H(F(0), F(1)), f.second);
  return Factor(H(F(0), F(0)), f.second);

}

void Monomial::proj()
{
  for(value_type::iterator iter = val.begin();
      iter != val.end(); ++iter) {
    *iter = make_pair((iter -> first).proj(iter -> second), iter -> second);
  }
}

Monomial::const_iterator Monomial::erase(const_iterator& it)
{
  return val.erase(it);
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

void S::omit()
{
  for(value_type::const_iterator iter = val.begin();
      iter != val.end();) {
    if(iter -> second == F(0))
      iter = val.erase(iter);
    else
      ++iter;
  }
}

S& operator*(const Action& a, S& v)
{
  for(Action::const_reverse_iterator riter = a.rbegin();
      riter != a.rend(); ++riter) {
    v += (*riter) * v;
  }
  return v;
}

S& operator*(const Actions& a, S& v)
{
  for(Actions::const_iterator iter = a.begin();
      iter != a.end(); ++iter) {
    v += (*iter) * v;
  }
  return v;
}

void Monomial::push(const Factor& f)
{
  val.push_back(::proj(f));
  sort();
}

Monomial append(const Factor& f, Monomial m)
{
  m.push(f);
  return m;
}

S append(const Factor& f, const S& v)
{
  S ret;
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    const Monomial& m = iter -> first;
    const F& coeff = (iter -> second);
    ret.insert(make_pair(append(f, m), coeff));
  }
  return ret;
}

Factor deriveTarget(int n)
{
  if (n <= 0) return Factor();
  if((n - 1) % 6 == 0)
    return Factor(H(F(0), F(1)), -n);
  if((n + 1) % 6 == 0)
    return Factor(H(F(1), F(0)), -n);
  return Factor();
}

S derive(Factor f, const S::Term& t)
{
  S ret;
  const F coeff = getCoeff(f);
  Factor target = deriveTarget(f.second);
  Monomial m = t.first;
  const unsigned int mul = count(m.begin(), m.end(), target);
  if(mul == 0) return ret;
  Monomial::const_iterator it = find(m.begin(), m.end(), target);
  m.erase(it);
  ret.insert(make_pair(m, coeff * t.second * F(mul * 3 * f.second) / F(CoxeterNum)));
  return ret;
}

S derive(const Factor& f, const S& v)
{
  S ret;
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    ret += derive(f, *iter);
  }
  return ret;
}

S operator*(const Factor& f, const S& v)
{
  if(f.second < 0)
    return append(f, v);
  if(f.second > 0)
    return derive(f, v);
  std::cerr << "Factor (*, 0) is not defined action of this realization." << std::endl;
  exit(1);
}
