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

ostream& operator<< (ostream& os, const Core& c)
{
  os << "[";
  for(Core::const_iterator iter = c.begin();
      iter != c.end(); ++iter) {
    if(iter != c.begin()) os << ", ";
    os << iter -> first
       << "("
       << iter ->second
       << ")";
  }
  return os << "]";
}

ostream& operator<< (ostream& os, const Monomial& m)
{
  os << "[";
  for(Core::const_iterator iter = m.begin();
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

bool operator< (const Core& lhs, const Core& rhs)
{
  if(lhs.size() != rhs.size()) return lhs.size() < rhs.size();
  Core::const_iterator it1 = lhs.begin();
  Core::const_iterator it2 = rhs.begin();
  while(it1 != lhs.end()) {
    if(it1 -> second != it2 -> second)
      return it1 -> second < it2 -> second;
    it1++;
    it2++;
  }
  return false;
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

void Core::sort()
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

void Core::proj()
{
  for(value_type::iterator iter = val.begin();
      iter != val.end(); ++iter) {
    *iter = make_pair((iter -> first).proj(iter -> second), iter -> second);
  }
}

Core::const_iterator Core::erase(const_iterator& it)
{
  return val.erase(it);
}

F Core::getCoeff() const
{
  F ret = F(1);
  for(Core::const_iterator iter = val.begin();
      iter != val.end(); ++iter) {
    ret *= (iter -> first).getProjVal(iter -> second);
  }
  return ret;
}

void Core::unifyCoeff()
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

S operator*(const Action& a, const S& v)
{
  S ret = v;
  for(Action::const_reverse_iterator riter = a.rbegin();
      riter != a.rend(); ++riter) {
    ret = (*riter) * ret;
  }
  return ret;
}

S operator*(const F& a, const S& v)
{
  S ret = v;
  for(S::iterator iter = ret.begin();
      iter != ret.end(); ++iter) {
    (iter -> second) *= a;
  }
  return ret;
}

S operator*(const Actions& a, const S& v)
{
  if(a.empty()) return S();
  S ret;
  for(Actions::const_iterator iter = a.begin();
      iter != a.end(); ++iter) {
    ret += (iter -> second) * ((iter -> first) * v);
  }
  return ret;
}

void Core::push_back(const Factor& f)
{
  val.push_back(::proj(f));
  sort();
}

Monomial append(const Factor& f, Monomial m)
{
  m.push_back(f);
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
  Core::const_iterator it = find(m.begin(), m.end(), target);
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

std::pair<Actions::iterator, bool> Actions::insert(Term x)
{
  auto ret = val.insert(unify(x));
  return ret;
}

Actions::Term& Actions::unify(Term& x)
{
  x.second *= x.first.getCoeff();
  x.first.unifyCoeff();
  return x;
}

F factorial(int n)
{
  if(n < 0) return F(0);
  if(n == 0) return F(1);
  return F(n) * factorial(n - 1);
}

void Actions::omit()
{
  for(value_type::const_iterator iter = val.begin();
      iter != val.end();) {
    if(iter -> second == F(0))
      iter = val.erase(iter);
    else
      ++iter;
  }
}

Actions operator*(const F& a, const Actions& v)
{
  Actions ret = v;
  for(Actions::iterator iter = ret.begin();
      iter != ret.end(); ++iter) {
    (iter -> second) *= a;
  }
  return ret;
}

Actions& Actions::operator+=(const Actions& rhs)
{
  for(Actions::const_iterator iter = rhs.begin();
      iter != rhs.end(); ++iter) {
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

Action operator*(Action lhs, const Action& rhs)
{
  for(Action::const_iterator iter = rhs.begin();
      iter != rhs.end(); ++iter) {
    lhs.push_back(*iter);
  }
  return lhs;
}

Actions& Actions::operator*=(const Actions& rhs)
{
  Actions::value_type ret;
  for(Actions::const_iterator it1 = val.begin();
      it1 != val.end(); ++it1) {
    for(Actions::const_iterator it2 = rhs.begin();
        it2 != rhs.end(); ++it2) {
      F coeff = (it1 -> second) * (it2 -> second);
      Action a = (it1 -> first) * (it2 -> first);
      value_type::iterator it = ret.find(a);
      if(it != ret.end()) {
        it -> second += coeff;
      }
      else
        ret.insert(Term(a, coeff));
    }
  }

  val = ret;
  omit();

  return *this;
}

Actions operator*(Actions lhs, const Actions& rhs)
{
  return lhs *= rhs;
}

void Operators::create()
{
  for(size_type i = MIN_DEG; i < MAX_DEG; ++i) {
    val[i] = Actions();
  }
}

void Operators::create(const F& f)
{
  val[DEG0] = Actions(f);
}

Operators& Operators::operator+=(const Operators& rhs)
{
  for(Operators::size_type i = MIN_DEG;
      i < MAX_DEG; ++i) {
    val[i] += rhs[i];
  }
  return *this;
}

Operators operator*(const F& lhs, const Operators& rhs)
{
  Operators ret;
  for(Operators::size_type i = Operators::MIN_DEG;
      i < Operators::MAX_DEG; ++i) {
    ret[i] = lhs * rhs[i];
  }
  return ret;
}

Operators operator*(const Operators& lhs, const Operators& rhs)
{
  using size_type = Operators::size_type;
  const auto MIN_DEG = Operators::MIN_DEG;
  const auto MAX_DEG = Operators::MAX_DEG;
  const auto DEG0 = Operators::DEG0;

  Operators ret;
  for(int i = MIN_DEG;
      i < MAX_DEG; ++i) {
    for(int j = MIN_DEG;
        j < MAX_DEG; ++j) {
      int target = i + j - DEG0;
      if(target >= MAX_DEG || target < MIN_DEG)
        continue;
      ret[target] += lhs[i] * rhs[j];
    }
  }
  return ret;
}

ostream& operator<< (ostream& os, const Actions& a)
{
  os << "[";
  for(Actions::const_iterator iter = a.begin();
      iter != a.end(); ++iter) {
    if(iter != a.begin())
      os << ", ";
    os << (iter -> second) << " * " << (iter -> first);
  }
  return os << "]";
}

ostream& operator<< (ostream& os, const Operators& op)
{
  for(int i = Operators::MIN_DEG; i < Operators::MAX_DEG; ++i) {
    if(i != Operators::MIN_DEG)
      os << std::endl;
    os << "z^" << i - int(Operators::DEG0) << " : " << std::endl
       << op.val[i];
  }
  return os;
}

Operators pow(const Operators& A, int n)
{
  if(n == 0) return Operators(F(1));
  if(n < 0) {
    std::cerr << "pow(Action A, int n) は n < 0 で未定義" << std::endl;
    exit(1);
  }
  if(n % 2 == 0) {
    Operators tmp = pow(A, n / 2);
    return tmp * tmp;
  }
  return A * pow(A, n - 1);
}

const Operators E_minus(const H& a)
{
  using std::size_t;
  Operators ret;
  Operators A;
  for(size_t i = Operators::DEG0 + 1; i < Operators::MAX_DEG; ++i) {
    A[i] = (F(CoxeterNum) / F(-i + Operators::DEG0)) * Actions(Factor(a, -i + Operators::DEG0));
  }
  for(int i = 0; i < (Operators::MAX_DEG - Operators::DEG0); ++i) {
    ret += (F(1) / factorial(i)) * pow(A, i);
  }
  return ret;
}

const Operators E_plus(const H& a)
{
  using std::size_t;
  Operators ret;
  Operators A;
  for(size_t i = Operators::DEG0 + 1; i < Operators::MAX_DEG; ++i) {
    A[-i + 2 * Operators::DEG0] = (F(CoxeterNum) / F(i - Operators::DEG0)) * Actions(Factor(a, i - Operators::DEG0));
  }
  for(int i = 0; i < (Operators::MAX_DEG - Operators::DEG0); ++i) {
    ret += (F(1) / factorial(i)) * pow(A, i);
  }
  return ret;
}

const Operators X(const H& a)
{
  return E_minus(-a) * E_plus(-a);
}
